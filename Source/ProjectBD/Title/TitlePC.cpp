// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.h"
#include "Kismet/GameplayStatics.h"

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

void ATitlePC::StartServer()
{
	//listen�� ���̸�
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("Listen"));
}

void ATitlePC::ConnectServer(FString ServerIPAddress)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerIPAddress));
}
