// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistance.h"
#include "ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombieAIController* AIC = Cast<AZombieAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(TEXT("Player")));
		AZombie* Zombie = AIC->GetPawn<AZombie>();
		if (Zombie) {
			float Distance = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());

			switch (TargetCondition)
			{
			case ECondition::LessThen: // <
			{
				if (Distance < TargetDistance) {
					Zombie->SetCurrentState(TargetState);
				}
			}
				break;
			case ECondition::GreaterThen: // >
			{
				if (Distance > TargetDistance) {
					Zombie->SetCurrentState(TargetState);
				}
			}
				break;
			}
		}
	}

	//Failed·Î ÇÏ¸é 
	return EBTNodeResult::Succeeded;
}
