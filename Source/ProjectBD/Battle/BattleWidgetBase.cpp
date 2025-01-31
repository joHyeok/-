﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBattleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount_WB")));
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar_WB")));
}

void UBattleWidgetBase::SetAliveCount(int NewCount)
{
	if (AliveCount)
	{
		FString Temp = FString::Printf(TEXT("%d명 생존"), NewCount);
		AliveCount->SetText(FText::FromString(Temp));
	}
}

void UBattleWidgetBase::UpdateHPBar(float Percent)
{
	if (HPBar)
	{
		HPBar->SetPercent(Percent);
	}
}
