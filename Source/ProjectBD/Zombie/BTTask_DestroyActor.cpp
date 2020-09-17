// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DestroyActor.h"
#include "ZombieAIController.h"

EBTNodeResult::Type UBTTask_DestroyActor::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC) {
		APawn* APawn = AIC->GetPawn();
		if (APawn) {
			APawn->Destroy();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
