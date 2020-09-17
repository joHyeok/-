// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Print.h"
#include "ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Basic/BasicPlayer.h"

void UBTService_Print::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AZombieAIController* AIC = Cast<AZombieAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		ABasicPlayer* Player = Cast<ABasicPlayer>(AIC->BBComponent->GetValueAsObject(TEXT("Player")));
		AZombie* Zombie = Cast<AZombie>(AIC->GetPawn());
		if (Zombie) {
			//Player->

		}
	}
}
