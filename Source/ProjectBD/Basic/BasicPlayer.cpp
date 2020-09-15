// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameframeWork/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BulletDamageType.h"
#include "Components/DecalComponent.h"
#include "MyCameraShake.h"


// Sets default values
ABasicPlayer::ABasicPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SocketOffset = FVector(0, 40.0f, 88.f);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));


	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	NormalSpringArmPosition = SpringArm->GetRelativeLocation();
	CrouchedSpringArmPosition = NormalSpringArmPosition + FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2);
}

// Called when the game starts or when spawned
void ABasicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABasicPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABasicPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABasicPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABasicPlayer::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABasicPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ABasicPlayer::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ABasicPlayer::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ABasicPlayer::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABasicPlayer::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABasicPlayer::StopFire);

	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &ABasicPlayer::StartIronsight);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Released, this, &ABasicPlayer::StopIronsight);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ABasicPlayer::StartCrouch);


	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABasicPlayer::Reload);


	PlayerInputComponent->BindAction(TEXT("LeftLean"), IE_Pressed, this, &ABasicPlayer::StartLeftLean);
	PlayerInputComponent->BindAction(TEXT("LeftLean"), IE_Released, this, &ABasicPlayer::StopLeftLean);

	PlayerInputComponent->BindAction(TEXT("RightLean"), IE_Pressed, this, &ABasicPlayer::StartRightLean);
	PlayerInputComponent->BindAction(TEXT("RightLean"), IE_Released, this, &ABasicPlayer::StopRightLean);


}

void ABasicPlayer::MoveForward(float AxisValue)
{
	//����ȸ��, ī�޶� ���� ����
	//GetControlRotation().Vector()
	//������ ���� (����)
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);
	//FRotator YawBaseRotation = FRotator(0, CameraRotation.Yaw, 0);

	FVector CameraForward = UKismetMathLibrary::GetForwardVector(YawBaseRotation);

	AddMovementInput(CameraForward, AxisValue);
}

void ABasicPlayer::MoveRight(float AxisValue)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	//FRotator YawBaseRotation = FRotator(0, CameraRotation.Yaw, 0);
	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);

	FVector CameraRight = UKismetMathLibrary::GetRightVector(YawBaseRotation);

	AddMovementInput(CameraRight, AxisValue);
}

void ABasicPlayer::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ABasicPlayer::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ABasicPlayer::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasicPlayer::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasicPlayer::StartFire()
{
	bIsFire = true;
	OnFire();
}

void ABasicPlayer::StopFire()
{
	bIsFire = false;
}

void ABasicPlayer::OnFire()
{
	if (!bIsFire)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		int32 ScreenSizeX;
		int32 ScreenSizeY;
		FVector CrosshairWorldPosition; //3D
		FVector CrosshairWorldDirection; //3D 

		FVector CameraLocation;
		FRotator CameraRotation;

		//��� �ݵ�
		int RandX = FMath::RandRange(-20, 20);
		int RandY = FMath::RandRange(3, 20);

		PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
		PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2 + RandX, ScreenSizeY / 2 + RandY, CrosshairWorldPosition, CrosshairWorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		//�ѱ� �鸮��(Tick���� �� �� �Ŀ� �ִϸ��̼�)
		FRotator PlayerRotation = GetControlRotation();
		PlayerRotation.Pitch += FMath::FRandRange(0.2f, 1.0f);
		GetController()->SetControlRotation(PlayerRotation);

		//ī�޶� ����
		//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UMyCameraShake::StaticClass());
		//���� ī�޶� ����
		//if (WorldCameraShake)
		//{
		//	UGameplayStatics::PlayWorldCameraShake(GetWorld(),
		//		WorldCameraShake,
		//		FVector(0, 0, 0),
		//		300.0f,
		//		900.0f,
		//		1.0f,
		//		false
		//	);
		//}


		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 99999.f);

		TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> ActorToIgnore;

		FHitResult OutHit;

		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			Objects,
			true,
			ActorToIgnore,
			EDrawDebugTrace::None,
			OutHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f
		);

		if (Result)
		{
			//HitEffect(Blood) and Decal
			if (Cast<ACharacter>(OutHit.GetActor()))
			{
				//ĳ����
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					BloodHitEffect,
					OutHit.ImpactPoint + (OutHit.ImpactNormal * 10)
				);

				//not use skeletalMesh
				//UDecalComponent* NewDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
				//	NormalDecal,
				//	FVector(5, 5, 5),
				//	OutHit.ImpactPoint,
				//	OutHit.ImpactNormal.Rotation(),
				//	10.0f
				//);

				//NewDecal->SetFadeScreenSize(0.005f);
			}
			else
			{
				//����
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					HitEffect,
					OutHit.ImpactPoint + (OutHit.ImpactNormal * 10)
				);

				UDecalComponent* NewDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
					NormalDecal,
					FVector(5, 5, 5),
					OutHit.ImpactPoint,
					OutHit.ImpactNormal.Rotation(),
					10.0f
				);

				NewDecal->SetFadeScreenSize(0.005f);
				
			}


			//�Ϲ� Damage
			//UGameplayStatics::ApplyDamage(OutHit.GetActor(), //������(Pawn)
			//	30.0f,	//������
			//	GetController(), // ���� �÷��̾� ��Ʈ�ѷ�
			//	this, //���� Pawn(�Ѿ�)
			//	UBulletDamageType::StaticClass()
			//);

			//RadialDamage
			//TArray<AActor*> IgnoreActors;
			//UGameplayStatics::ApplyRadialDamage(GetWorld(), // ������ ����
			//	30.0f,	//��������
			//	OutHit.ImpactPoint, //������ �߻� ��ġ
			//	500.0f,	//������ ����
			//	UBulletDamageType::StaticClass(), //������ Ÿ��
			//	IgnoreActors,	//������ ������Ʈ
			//	this,	//������
			//	GetController(),	//���� �÷��̾�
			//	true,	//Ǯ ������
			//	ECC_Visibility	//������ �����ִ� ä�� ����(�浹ü)
			//);

			//Point Damage
			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), //������
				1.0f,	//������
				-OutHit.ImpactNormal,	//������ ����
				OutHit,	//������ �浹 ����
				GetController(),	//���� �÷��̾�
				this,	//������
				UBulletDamageType::StaticClass() //������ Ÿ��
			);

			MakeNoise(1.0f, this, OutHit.ImpactPoint);
		}
	}

	//WeaponSound and MuzzleFlash
	if (WeaponSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
			WeaponSound,
			Weapon->GetComponentLocation()
		);
	}

if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			MuzzleFlash,
			Weapon->GetSocketTransform(TEXT("Muzzle"))
		);
	}


	GetWorldTimerManager().SetTimer(FireTimerHandle,
		this,
		&ABasicPlayer::OnFire,
		0.12f,
		false);
}

void ABasicPlayer::StartIronsight()
{
	bIsIronsight = true;
}

void ABasicPlayer::StopIronsight()
{
	bIsIronsight = false;
}

void ABasicPlayer::StartCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}


//������ ó���Ǵ� ��� ���Ϳ� ������ �ؾ� ��
float ABasicPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //PointDamage ó��
	{
		//������
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Point Damage %s"), *(PointDamageEvent->HitInfo.BoneName.ToString()));

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		} 
		else
		{
			CurrentHP -= DamageAmount;
		}

		if (HitActionMontage)
		{
			FString SectionName = FString::Printf(TEXT("Hit%d"), FMath::RandRange(1, 4));
			PlayAnimMontage(HitActionMontage, 1.0f, FName(SectionName));
		}


		CurrentHP = FMath::Clamp(CurrentHP, 0.0f, 100.0f);

		if (CurrentHP <= 0)
		{
			//�״°�
			//�ִϸ��̼�
			if (DeadMontage)
			{
				FString SectionName = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
				PlayAnimMontage(DeadMontage, 1.0f, FName(SectionName));
			}

			//���� ó��
			//GetMesh()->SetSimulatePhysics(true);
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//GetMesh()->AddImpulse(-PointDamageEvent->HitInfo.ImpactNormal * 30000.0f, PointDamageEvent->HitInfo.BoneName);
		}

	} 
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) //RadialDamage ó��
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Radial Damage %f"), DamageAmount);
	}
	else //��� ������ ó��
	{
		CurrentHP -= DamageAmount;

	}



	UE_LOG(LogClass, Warning, TEXT("%f"), DamageAmount);

	return 0.0f;
}

void ABasicPlayer::Reload()
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (ReloadMontage && AnimInstance)
	{
		if (!AnimInstance->Montage_IsPlaying(ReloadMontage))
		{
			PlayAnimMontage(ReloadMontage);
		}
	}
}

void ABasicPlayer::StartLeftLean()
{
	bLeftLean = true;
}

void ABasicPlayer::StopLeftLean()
{
	bLeftLean = false;
}

void ABasicPlayer::StartRightLean()
{
	bRightLean = true;
}

void ABasicPlayer::StopRightLean()
{
	bRightLean = false;
}

FRotator ABasicPlayer::GetAimOffset() const
{
	//WorldSpace Rotation -> LocalSpace Rotation, Rotation World -> Local
	//WorldSpace Rotation -> WorldSpace Direction Vector -> LocalSpace Direction Vector
	//LocalSpace Direction Vector -> LocalSpace Rotation

	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
	//return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
}

