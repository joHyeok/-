// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UScrollBox* ItemSlots;

	void UpdateInventory(TArray<class AMasterItem*> Inventory);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UItemSlotBase> ItemSlotClass;

	//TArray<class UItemSlotBase*> ItemSlotLists;

	int GetEmptySlot();
	void SetSlot(int Index, class AMasterItem* Item);
	
	
};
