// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UItemSlotBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBorder* ItemThumnail;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTextBlock* ItemName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTextBlock* ItemCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UButton* ItemButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnItemButton();
	void UpdateItemSlot(class AMasterItem* Item);

	class AMasterItem* SelectItem;

	bool bUse = false;
};
