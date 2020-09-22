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
#include "Net/Unrealnetwork.h"
#include "../Battle/BattleWidgetBase.h"
#include "../Battle/BattlePC.h"
#include "../Battle/BattleGM.h"
#include "../Item/MasterItem.h"


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

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void ABasicPlayer::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;

	//UI가 있는지 체크 확인 필수
	OnRep_CurrentHP();
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

	PlayerInputComponent->BindAction(TEXT("Pickup"), IE_Pressed, this, &ABasicPlayer::Pickup);

	//PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ABasicPlayer::ToggleInventory);
}

void ABasicPlayer::ToggleInventory()
{
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		PC->ToggleInventory();
	}
}

void ABasicPlayer::MoveForward(float AxisValue)
{
	//절대회전, 카메라 같게 설정
	//GetControlRotation().Vector()
	//수학의 나라 (엔진)
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
	//Client
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	//Server
	C2S_SetSprint(true);
}

void ABasicPlayer::StopSprint()
{
	//Client
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//Server
	C2S_SetSprint(false);

}

void ABasicPlayer::C2S_SetFire_Implementation(bool State)
{
	//서버에서 실행하는 함수여서 이 구문은 서버의 플레이어의 bIsFire가 변화한다
	bIsFire = State;
}

void ABasicPlayer::C2S_SetSprint_Implementation(bool State)
{
	bIsSprint = State;
	GetCharacterMovement()->MaxWalkSpeed = State ? SprintSpeed : WalkSpeed;
}

void ABasicPlayer::C2S_SetIronsight_Implementation(bool State)
{
	bIsIronsight = State;
}

void ABasicPlayer::StartFire()
{
	//내거 업데이트하고
	bIsFire = true;
	//서버도 업데이트하고 다시 받아서 한다.
	//안그러면 딜레이가 생긴다.
	C2S_SetFire(true);
	OnFire();
}

void ABasicPlayer::StopFire()
{
	bIsFire = false;
	C2S_SetFire(false);
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

		//사람 반동
		int RandX = FMath::RandRange(-20, 20);
		int RandY = FMath::RandRange(3, 20);

		PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
		PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2 + RandX, ScreenSizeY / 2 + RandY, CrosshairWorldPosition, CrosshairWorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		//총구 들리기(Tick에서 총 쏜 후에 애니메이션)
		FRotator PlayerRotation = GetControlRotation();
		PlayerRotation.Pitch += FMath::FRandRange(0.2f, 1.0f);
		GetController()->SetControlRotation(PlayerRotation);

		//카메라 흔들기
		//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UMyCameraShake::StaticClass());
		//월드 카메라 흔들기
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

		C2S_ProcessFire(TraceStart, TraceEnd);
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
	C2S_SetIronsight(true);
}

void ABasicPlayer::StopIronsight()
{
	bIsIronsight = false;
	C2S_SetIronsight(false);
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

//Host Only
//데미지 처리되는 모든 액터에 구현을 해야 됨
float ABasicPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //PointDamage 처리
	{
		//서버에서 클라이언트로 데미지 전달
		//S2A_ProcessTakeDamage(DamageAmount, DamageEvent);
		//이건 필요가 없다. CurrentHP가 일단 전달되기 때문에 HP에 따라서 죽는애니메이션 재생할 수 있다.

		//다형성
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

		//UI의 HPBar 업데이트
		OnRep_CurrentHP();

		//서버에서 모든 클라이언트로 Hit모션 보내기
		//랜덤값은 서버에서 결정해서 보낸다
		//안그러면 클라이언트마다 다른 랜덤값을 갖게 되서 다른 모션이 된다.
		S2A_HitActionMontage(FMath::RandRange(1, 4));

		CurrentHP = FMath::Clamp(CurrentHP, 0.0f, 100.0f);

		if (CurrentHP <= 0)
		{
			//죽는거
			//애니메이션
			S2A_DeadMontage(FMath::FRandRange(1,3));

			ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				GM->CountAlivePlayer();
			}
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) //RadialDamage 처리
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Radial Damage %f"), DamageAmount);
	}
	else //모든 데미지 처리
	{
		CurrentHP -= DamageAmount;
	}

	UE_LOG(LogClass, Warning, TEXT("%f"), DamageAmount);

	return 0.0f;
}

void ABasicPlayer::OnRep_CurrentHP()
{
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	//다른 클라이언트에서의 HP가 변하지 않게 IsLocalController를 확인해준다.
	if (PC && PC->IsLocalController())
	{
		if (PC->BattleWidgetObject)
		{
			PC->BattleWidgetObject->UpdateHPBar(CurrentHP / MaxHP);

		}
	}
}

void ABasicPlayer::Reload()
{
	bIsReload = true;

	C2S_SetReload(bIsReload);
}

void ABasicPlayer::StartLeftLean()
{
	bLeftLean = true;
	C2S_SetLeftLean(true);
}

void ABasicPlayer::StopLeftLean()
{
	bLeftLean = false;
	C2S_SetLeftLean(false);
}

void ABasicPlayer::StartRightLean()
{
	bRightLean = true;
	C2S_SetRightLean(true);
}

void ABasicPlayer::StopRightLean()
{
	bRightLean = false;
	C2S_SetRightLean(false);
}

void ABasicPlayer::C2S_SetLeftLean_Implementation(bool State)
{
	bLeftLean = State;
}

void ABasicPlayer::C2S_SetRightLean_Implementation(bool State)
{
	bRightLean = State;
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

void ABasicPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicPlayer, bIsFire);
	DOREPLIFETIME(ABasicPlayer, bIsSprint);
	DOREPLIFETIME(ABasicPlayer, bIsIronsight);
	DOREPLIFETIME(ABasicPlayer, CurrentHP);
	DOREPLIFETIME(ABasicPlayer, MaxHP);
	DOREPLIFETIME(ABasicPlayer, bLeftLean);
	DOREPLIFETIME(ABasicPlayer, bRightLean);
	DOREPLIFETIME(ABasicPlayer, bIsReload);
}

void ABasicPlayer::C2S_ProcessFire_Implementation(FVector TraceStart, FVector TraceEnd)
{
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
		S2A_SpawnHitEffectAndDecal(OutHit);

		//일반 Damage
		//UGameplayStatics::ApplyDamage(OutHit.GetActor(), //맞은놈(Pawn)
		//	30.0f,	//데미지
		//	GetController(), // 때린 플레이어 컨트롤러
		//	this, //때린 Pawn(총알)
		//	UBulletDamageType::StaticClass()
		//);

		//RadialDamage
		//TArray<AActor*> IgnoreActors;
		//UGameplayStatics::ApplyRadialDamage(GetWorld(), // 데미지 월드
		//	30.0f,	//데미지량
		//	OutHit.ImpactPoint, //데미지 발생 위치
		//	500.0f,	//데미지 범위
		//	UBulletDamageType::StaticClass(), //데미지 타입
		//	IgnoreActors,	//무시할 오브젝트
		//	this,	//때린놈
		//	GetController(),	//때린 플레이어
		//	true,	//풀 데미지
		//	ECC_Visibility	//데미지 막아주는 채널 정보(충돌체)
		//);

		//Point Damage
		UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), //맞은놈
			1.0f,	//데미지
			-OutHit.ImpactNormal,	//데미지 방향
			OutHit,	//데미지 충돌 정보
			GetController(),	//때린 플레이어
			this,	//때린놈
			UBulletDamageType::StaticClass() //데미지 타입
		);

		MakeNoise(1.0f, this, OutHit.ImpactPoint);
	}
	
	//서버에서 모든 클라이언트로 보내기
	S2A_SpawnMuzzleFlashAndSound();
}

void ABasicPlayer::S2A_SpawnMuzzleFlashAndSound_Implementation()
{
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
}

void ABasicPlayer::S2A_SpawnHitEffectAndDecal_Implementation(FHitResult OutHit)
{
	//HitEffect(Blood) and Decal
	if (Cast<ACharacter>(OutHit.GetActor()))
	{
		//캐릭터
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
		//지형
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
}

void ABasicPlayer::S2A_HitActionMontage_Implementation(int Number)
{
	if (HitActionMontage)
	{
		FString SectionName = FString::Printf(TEXT("Hit%d"), Number);
		PlayAnimMontage(HitActionMontage, 1.0f, FName(SectionName));
	}
}

void ABasicPlayer::S2A_DeadMontage_Implementation(int Number)
{
	if (DeadMontage)
	{
		FString SectionName = FString::Printf(TEXT("Death_%d"), Number);
		PlayAnimMontage(DeadMontage, 1.0f, FName(SectionName));

		//물리 처리
			//GetMesh()->SetSimulatePhysics(true);
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//GetMesh()->AddImpulse(-PointDamageEvent->HitInfo.ImpactNormal * 30000.0f, PointDamageEvent->HitInfo.BoneName);
	}

	DisableInput(Cast<APlayerController>(GetController()));
}

void ABasicPlayer::C2S_SetReload_Implementation(bool newState)
{
	bIsReload = newState;
}

void ABasicPlayer::AddPickItem(AMasterItem * AddItem)
{
	PickItemList.Add(AddItem);
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		PC->ShowItemTooltip(AddItem->ItemData.ItemName);
	}
}

void ABasicPlayer::RemovePickItem(AMasterItem * RemoveItem)
{
	PickItemList.Remove(RemoveItem);
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		//겹친 아이템에서 두번째로 들어온 아이템이름을 보여주고
		//두번째의 콜리전에서 나올경우 아직 겹쳐있는 첫번째 아이템의 이름을 보여야한다.
		//그래서 리스트에 아직 남아있는 첫번째 아이템을 검사하고 리스트맴버가 0보다 큰지
		//리스트의 현 넘버 -1번째의 이름을 보여준다. 그게 첫번째 아이템
		if (PickItemList.Num() > 0)
		{
			PC->ShowItemTooltip(PickItemList[PickItemList.Num() - 1]->ItemData.ItemName);
		}
		else
		{
			PC->HideItemTooltip();
		}
		
	}
}

void ABasicPlayer::Pickup()
{
	if (PickItemList.Num() > 0)
	{
		//Server Pickup check
		C2S_CheckPickupItem(PickItemList[PickItemList.Num() - 1]);
	}
}

void ABasicPlayer::C2S_CheckPickupItem_Implementation(AMasterItem * PickupItem)
{
	//네트워크에서 이 아이템이 지워졌는지 검사하는 IsPendingKill()
	//아이템이 지워졌는지 네트워크 상에서 딜레이가 있어서 이거로 검사한다.
	if (PickupItem && !PickupItem->IsPendingKill())
	{
		S2C_InsertItem(PickupItem);
		PickupItem->Destroy();
	}
}

void ABasicPlayer::S2C_InsertItem_Implementation(AMasterItem * PickupItem)
{
	AddInventory(PickupItem);
}

void ABasicPlayer::AddInventory(AMasterItem * Item)
{
	Inventory.Add(Item);
}

void ABasicPlayer::RemoveInventory(AMasterItem * Item)
{
	Inventory.Remove(Item);
}

