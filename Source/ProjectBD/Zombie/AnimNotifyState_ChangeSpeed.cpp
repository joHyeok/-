// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ChangeSpeed.h"
#include "Zombie.h"

void UAnimNotifyState_ChangeSpeed::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
}

void UAnimNotifyState_ChangeSpeed::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->SetSpeed(3.f);
	}
}

void UAnimNotifyState_ChangeSpeed::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->SetSpeed(100.f);
	}
}
