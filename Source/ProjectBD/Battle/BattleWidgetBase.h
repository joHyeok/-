// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UBattleWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTextBlock* AliveCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UProgressBar* HPBar;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetAliveCount(int NewCount);

	UFUNCTION(BlueprintCallable)
	void UpdateHPBar(float Percent);

	
};
