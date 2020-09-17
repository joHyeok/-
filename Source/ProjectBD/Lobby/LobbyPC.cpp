// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "../Basic/BasicPCM.h"


ALobbyPC::ALobbyPC()
{
	PlayerCameraManagerClass = ABasicPCM::StaticClass();
}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();
	
	//경로설정
	FSoftClassPath LobbyWidgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/Lobby/LobbyWidget.LobbyWidget_C'"));
	//경로를 가지고 #include함
	UClass* WidgetClass = LobbyWidgetClass.TryLoadClass<ULobbyWidgetBase>();

	if (WidgetClass)
	{
		//UI만들려고 할때 자기 로컬 플레이어 컨트롤러인지 확인한다
		// 다른 플레이어의 컨트롤러이면 UI를 만들 필요 없기 때문이다
		//이 확인을 안하면 에러가 난다.
		if (IsLocalPlayerController())
		{
			//위젯 만듬
			LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, WidgetClass);
			if (LobbyWidgetObject)
			{
				LobbyWidgetObject->AddToViewport();
			}

			//마우스와 키보드 입력 동시받기
			SetInputMode(FInputModeGameAndUI());
			bShowMouseCursor = true;
		}
		
	}

	//TSubclassOf로 했을경우
	//LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);


}

bool ALobbyPC::C2S_SendMessage_Validate(const FText& Message)
{
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(const FText& Message)
{
	//c++ 11
	//auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++ 중의 조건이
	//Iter; 인데 존재한다면 이므로 null이 되면 중단 즉 컨트롤러가 더 없으면 중단

	//서버에서 플레이어 컨트롤러들을 찾아서 클라이언트에서 보내온 채팅내용을 보낸다
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			//Server to Client
			PC->S2C_SendMessage(Message);
		}
	}
}

//Call Server, Execute Client
void ALobbyPC::S2C_SendMessage_Implementation(const FText & Message)
{
	if (LobbyWidgetObject)
	{
		//위젯에 입력
		LobbyWidgetObject->AddMessage(Message);
	}
}
