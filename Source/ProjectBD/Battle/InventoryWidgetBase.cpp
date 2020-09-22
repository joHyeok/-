// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "Components/ScrollBox.h"
#include "ItemSlotBase.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemSlots = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemSlots")));

}

void UInventoryWidgetBase::UpdateInventory(TArray<class AMasterItem*> Inventory)
{
	//for (int i = ItemSlots->GetChildrenCount(); i > 0; --i)
	//{
	//	//ItemSlots->RemoveChildAt(i);
	//	ItemSlots->GetChildAt(i)
	//}

	//각 슬롯 초기화
	for (int i = 0; i < ItemSlots->GetChildrenCount(); ++i)
	{
		UItemSlotBase* InventorySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(i));
		if (InventorySlot)
		{
			InventorySlot->bUse = false;
			InventorySlot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//슬롯 활성화 및 아이템 넣기
	for (int i = 0; i < Inventory.Num(); ++i)
	{
		//UItemSlotBase* NewItem = NewObject<UItemSlotBase>(ItemSlotClass);
		//if (NewItem)
		//{
		//	NewItem->UpdateItemSlot(Inventory[i]);
		//	if (ItemSlots)
		//	{
		//		ItemSlots->AddChild(NewItem);
		//	}
		//}

		//CreateWidget으로 만들어도 된다.
		//하지만 만들어 놓고 풀링하는 방식이 메모리 관점에서도 더 좋다.
		//CreateWidget<UItemSlotBase>(this, ItemSlotClass);
		//NewObject는 초기화도 스스로 해줘야한다.
		//용도에 따라 만들어둔 CreateWidget으로 하면 위젯을 만들고 초기화도 해준다.
		//어떤 방법을 쓸지는 맘대로

		int FindIndex = GetEmptySlot();
		if (FindIndex > -1)
		{
			SetSlot(FindIndex, Inventory[i]);
		}
	}

}

int UInventoryWidgetBase::GetEmptySlot()
{
	for (int i = 0; i < ItemSlots->GetChildrenCount(); ++i)
	{
		UItemSlotBase* EmptySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(i));
		if (EmptySlot && EmptySlot->bUse == false)
		{
			return i;
		}
	}
	return -1;	//full
}

void UInventoryWidgetBase::SetSlot(int Index, AMasterItem * Item)
{

	UItemSlotBase* EmptySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(Index));
	if (EmptySlot && EmptySlot->bUse == false)
	{
		EmptySlot->UpdateItemSlot(Item);
		EmptySlot->bUse = true;
		EmptySlot->SetVisibility(ESlateVisibility::Visible);
	}

}
