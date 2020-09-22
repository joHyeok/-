// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "BattleWidgetBase.h"
#include "../Basic/BasicPCM.h"
#include "../Item/ItemTooltipBase.h"
#include "InventoryWidgetBase.h"
#include "Components/InputComponent.h"
#include "../Basic/BasicPlayer.h"

ABattlePC::ABattlePC()
{
	PlayerCameraManagerClass = ABasicPCM::StaticClass();
}

void ABattlePC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		BattleWidgetObject = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
		if (BattleWidgetObject)
		{
			BattleWidgetObject->AddToViewport();
		}
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		ItemTooltipObject = CreateWidget<UItemTooltipBase>(this, ItemTooltipClass);
		if (ItemTooltipObject)
		{
			ItemTooltipObject->AddToViewport();
			HideItemTooltip();
		}

		InventoryWidgetObject = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
		if (InventoryWidgetObject)
		{
			InventoryWidgetObject->AddToViewport();
			HideInventory();
		}
	}


}

void ABattlePC::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ABattlePC::ToggleInventory);
}

void ABattlePC::ToggleInventory()
{
	if (InventoryWidgetObject)
	{
		if (InventoryWidgetObject->GetVisibility() == ESlateVisibility::Visible)
		{
			HideInventory();
		}
		else
		{
			ABasicPlayer* ItemPawn = Cast<ABasicPlayer>(GetPawn());
			if (ItemPawn)
			{
				InventoryWidgetObject->UpdateInventory(ItemPawn->Inventory);
				ShowInventory();
			}

		}
	}
}

void ABattlePC::ShowItemTooltip(FString ItemName)
{
	if (ItemTooltipObject)
	{
		ItemTooltipObject->SetItemName(ItemName);
		ItemTooltipObject->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABattlePC::HideItemTooltip()
{
	if (ItemTooltipObject)
	{
		ItemTooltipObject->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABattlePC::ShowInventory()
{
	if (InventoryWidgetObject)
	{
		InventoryWidgetObject->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void ABattlePC::HideInventory()
{
	if (InventoryWidgetObject)
	{
		InventoryWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}