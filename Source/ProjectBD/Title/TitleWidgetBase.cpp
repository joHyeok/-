// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "TitlePC.h"
#include "../BDGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidgetBase::NativeConstruct()
{
	//에디터의 위젯 가져오기
	UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserID")));
	Passward = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Passward")));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));

	//위젯 이름을 못찾
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnConnectButton);
	}

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnStartButton);
	}
}

void UTitleWidgetBase::OnConnectButton()
{
	//서버 접속
	ATitlePC* PC = GetOwningPlayer<ATitlePC>();
	if (PC)
	{
		SaveUserID();

		FString ServerIPAddress = ServerIP->GetText().ToString();
		PC->ConnectServer(ServerIPAddress);
	}
}

void UTitleWidgetBase::OnStartButton()
{
	//서버 시작
	ATitlePC* PC = GetOwningPlayer<ATitlePC>();

	if (PC)
	{
		SaveUserID();

		PC->StartServer();
	}

}

void UTitleWidgetBase::SaveUserID()
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		if (UserID)
		{
			//GI의 UserID에 UserID 위젯의 텍스트를 넣어주기
			GI->SetUserID(UserID->GetText().ToString());
		}
	}
}
