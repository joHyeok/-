// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.h"

void ATitlePC::BeginPlay()
{
	if (TitleWidgetClass)
	{
		//(UI ������ �ִ³� : �÷��̾��� : �÷��̾� ��Ʈ�ѷ�, Ŭ����)
		TitleWidgetObject = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidgetObject->AddToViewport();

		//�÷��̾���Ʈ�ѷ��� �����Ѵ�
		bShowMouseCursor = true;

		SetInputMode(FInputModeUIOnly());
	}
}
