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

	//�� ���� �ʱ�ȭ
	for (int i = 0; i < ItemSlots->GetChildrenCount(); ++i)
	{
		UItemSlotBase* InventorySlot = Cast<UItemSlotBase>(ItemSlots->GetChildAt(i));
		if (InventorySlot)
		{
			InventorySlot->bUse = false;
			InventorySlot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//���� Ȱ��ȭ �� ������ �ֱ�
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

		//CreateWidget���� ���� �ȴ�.
		//������ ����� ���� Ǯ���ϴ� ����� �޸� ���������� �� ����.
		//CreateWidget<UItemSlotBase>(this, ItemSlotClass);
		//NewObject�� �ʱ�ȭ�� ������ ������Ѵ�.
		//�뵵�� ���� ������ CreateWidget���� �ϸ� ������ ����� �ʱ�ȭ�� ���ش�.
		//� ����� ������ �����

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
