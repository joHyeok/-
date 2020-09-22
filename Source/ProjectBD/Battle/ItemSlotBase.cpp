// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "../Item/MasterItem.h"
#include "Engine/StreamableManager.h"
#include "../Basic/BasicPlayer.h"

void UItemSlotBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemThumnail = Cast<UBorder>(GetWidgetFromName(TEXT("ItemThumnail")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));

	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UItemSlotBase::OnItemButton);
	}
}

void UItemSlotBase::OnItemButton()
{
	bUse = false;
	SetVisibility(ESlateVisibility::Collapsed);

	ABasicPlayer* Pawn = GetOwningPlayerPawn<ABasicPlayer>();
	if (Pawn)
	{
		Pawn->RemoveInventory(SelectItem);
	}
}

void UItemSlotBase::UpdateItemSlot(AMasterItem* Item)
{
	if (Item)
	{
		FStreamableManager Loader;
		ItemThumnail->SetBrushFromTexture(Loader.LoadSynchronous<UTexture2D>(Item->ItemData.ItemThumnail));
		ItemName->SetText(FText::FromString(Item->ItemData.ItemName));
		ItemCount->SetText(FText::FromString(FString::FromInt(Item->ItemData.ItemCount)));

		SelectItem = Item;
	}
}
