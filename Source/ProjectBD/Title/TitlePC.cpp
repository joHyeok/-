// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.h"

void ATitlePC::BeginPlay()
{
	if (TitleWidgetClass)
	{
		//(UI 가지고 있는놈 : 플레이어임 : 플레이어 컨트롤러, 클래스)
		TitleWidgetObject = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidgetObject->AddToViewport();

		//플레이어컨트롤러가 관리한다
		bShowMouseCursor = true;

		SetInputMode(FInputModeUIOnly());
	}
}
