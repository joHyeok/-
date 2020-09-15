// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Zombie.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//Super를 할필요가 없다 왜냐하면 얘가 가장 부모여서

	//사실 GetAIOwner()가 AAIController타입이라 캐스팅 안해도 됨
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());

	if (AIC) {
		//소유한 pawn을 좀비로 캐스팅해서 가져옴
		AZombie* Zombie = AIC->GetPawn<AZombie>();
		if (Zombie) {
			Zombie->SetSpeed(Speed);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
