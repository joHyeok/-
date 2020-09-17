// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayerAttack.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	UE_LOG(LogClass, Warning, TEXT("PlayerAttack"));
}
