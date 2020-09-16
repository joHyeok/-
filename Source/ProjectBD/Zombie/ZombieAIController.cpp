// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void AZombieAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	//AIController가 ZOmbie를 잡으면 그때 BT정보를 초기화해서 사용
	AZombie* Zombie = Cast<AZombie>(InPawn);
	if (Zombie) {
		if (Zombie->ZombieBT && Zombie->ZombieBT->BlackboardAsset) {
			//인자 타입이 하나는 레퍼런스고 하나는 포인터라 저렇게 넣는다
			BBComponent->InitializeBlackboard(*(Zombie->ZombieBT->BlackboardAsset));
			BTComponent->StartTree(*(Zombie->ZombieBT));
		}
	}
}

void AZombieAIController::OnUnPossess()
{
	Super::OnUnPossess();
	BTComponent->StopTree();
}

void AZombieAIController::SetCurrentState(EZombieState NewState) {
	//Blackboard에 입력
	if (BBComponent) {
		BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)NewState);
	}
}

void AZombieAIController::SetTarget(FVector NewLocation)
{
	if (BBComponent) {
		BBComponent->SetValueAsVector(TEXT("Target"), NewLocation);
	}
}

void AZombieAIController::SetPlayer(AActor * Player)
{
	if (BBComponent) {
		BBComponent->SetValueAsObject(TEXT("Player"), Player);
	}
}
