// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGS.h"
#include "Net/UnrealNetwork.h"
#include "BattlePC.h"
#include "Kismet/GameplayStatics.h"
#include "BattleWidgetBase.h"

void ABattleGS::OnRep_AliveCount()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC && PC->BattleWidgetObject)
	{
		PC->BattleWidgetObject->SetAliveCount(AlivePlayer);
	}
}

void ABattleGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABattleGS, AlivePlayer);
	DOREPLIFETIME(ABattleGS, TotalPlayerCount);
}
