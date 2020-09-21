// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattleGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing="OnRep_AliveCount", Category = "Network")
	int AlivePlayer = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated, Category = "Network")
	int TotalPlayerCount = 0;

	UFUNCTION()
	void OnRep_AliveCount();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
