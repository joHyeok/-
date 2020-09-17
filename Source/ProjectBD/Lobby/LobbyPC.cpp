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
	
	//��μ���
	FSoftClassPath LobbyWidgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/Lobby/LobbyWidget.LobbyWidget_C'"));
	//��θ� ������ #include��
	UClass* WidgetClass = LobbyWidgetClass.TryLoadClass<ULobbyWidgetBase>();

	if (WidgetClass)
	{
		//UI������� �Ҷ� �ڱ� ���� �÷��̾� ��Ʈ�ѷ����� Ȯ���Ѵ�
		// �ٸ� �÷��̾��� ��Ʈ�ѷ��̸� UI�� ���� �ʿ� ���� �����̴�
		//�� Ȯ���� ���ϸ� ������ ����.
		if (IsLocalPlayerController())
		{
			//���� ����
			LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, WidgetClass);
			if (LobbyWidgetObject)
			{
				LobbyWidgetObject->AddToViewport();
			}

			//���콺�� Ű���� �Է� ���ùޱ�
			SetInputMode(FInputModeGameAndUI());
			bShowMouseCursor = true;
		}
		
	}

	//TSubclassOf�� �������
	//LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);


}

bool ALobbyPC::C2S_SendMessage_Validate(const FText& Message)
{
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(const FText& Message)
{
	//c++ 11
	//auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++ ���� ������
	//Iter; �ε� �����Ѵٸ� �̹Ƿ� null�� �Ǹ� �ߴ� �� ��Ʈ�ѷ��� �� ������ �ߴ�

	//�������� �÷��̾� ��Ʈ�ѷ����� ã�Ƽ� Ŭ���̾�Ʈ���� ������ ä�ó����� ������
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
		//������ �Է�
		LobbyWidgetObject->AddMessage(Message);
	}
}
