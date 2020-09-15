// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BasicHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABasicHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void LoadHUDTexture();

	class UTexture2D* Crosshair;

	FSoftObjectPath T_Crosshair;
};
