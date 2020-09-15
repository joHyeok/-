// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAnimInstance.h"
#include "BasicPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ABasicPlayer* Pawn = Cast<ABasicPlayer>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity,
			Pawn->GetActorRotation());

		Speed = Pawn->GetCharacterMovement()->Velocity.Size();

		bIsCrouched = Pawn->bIsCrouched;
		bIsFire = Pawn->bIsFire;
		bIsSprint = Pawn->bIsSprint;
		bIsIronsight = Pawn->bIsIronsight;

		//기울이기 기능, 본을 회전 시켜서
		float TargetLeanAngle;
		if (Pawn->bLeftLean && !Pawn->bRightLean)
		{
			TargetLeanAngle = -30.0f;
		}
		else if (Pawn->bRightLean && !Pawn->bLeftLean)
		{
			TargetLeanAngle = 30.0f;
		}
		else
		{
			TargetLeanAngle = 0;
		}
		CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, TargetLeanAngle, DeltaSeconds,
			8.0f);

		FRotator AimRotation = Pawn->GetAimOffset();
		AimYaw = AimRotation.Yaw;
		AimPitch = AimRotation.Pitch;
	}
}
