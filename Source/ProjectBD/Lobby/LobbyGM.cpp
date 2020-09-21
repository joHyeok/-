// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"

void ALobbyGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->ConnectCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			GS->ConnectCount++;
			GS->OnRep_ConnectCount(); //Host Only
		}
	}
}

void ALobbyGM::Logout(AController * Exiting)
{
	CountConnectCount();

	//Super�� ���� ���� �ȵȴ�.
	// Super�ϸ� �����鼭 �÷��̾ ����������.
	// ���� �Լ��� ���� ��Ű�� ���� Super�ؼ� ������
	Super::Logout(Exiting);
}

void ALobbyGM::CountConnectCount()
{
	//�α׾ƿ� �ص� ��Ʈ�ѷ��� ��������� ����� �ȳ��´�
	//�� ���� �غ���
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->ConnectCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			GS->ConnectCount++;
			GS->OnRep_ConnectCount(); //Host Only
		}
	}
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(LobbyTimer, this, &ALobbyGM::DecreaseTime, 1.0f, true, 1.0f);
}

void ALobbyGM::DecreaseTime()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->LeftTime--;
		GS->OnRep_LeftTime(); //Host Only

		if (GS->LeftTime < 0)
		{
			//���� ����
			StartGame();
		}
	}
}

void ALobbyGM::StartGame()
{
	//OpenLevel�� ó������ ����ϰ� ���Ŀ��� �������� ���� ����
	GetWorld()->ServerTravel(TEXT("Battle"));
}
