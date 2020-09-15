// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPCM.h"
#include "BasicPlayer.h"
#include "GameFramework/SpringArmComponent.h"

ABasicPCM::ABasicPCM()
{
}

void ABasicPCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	ABasicPlayer* Pawn = GetOwningPlayerController()->GetPawn<ABasicPlayer>();

	if (Pawn)
	{
		//Ironsight
		float TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
	
		//float ResultFOV = FMath::Lerp(GetFOVAngle(), TargetFOV, 0.1f);
		float ResultFOV = FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 20.0f);
		
		SetFOV(ResultFOV);

		//CameraPosition
		FVector TargetPosition = Pawn->bIsCrouched ? Pawn->CrouchedSpringArmPosition : Pawn->NormalSpringArmPosition;

		FVector ResultSpringArmPosition = FMath::VInterpTo(Pawn->SpringArm->GetRelativeLocation(), TargetPosition, DeltaTime, 15.0f);

		Pawn->SpringArm->SetRelativeLocation(ResultSpringArmPosition);
	}
}
