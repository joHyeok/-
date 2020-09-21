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

	//Super를 먼저 쓰면 안된다.
	// Super하면 나가면서 플레이어를 날려버린다.
	// 실행 함수를 실행 시키고 나서 Super해서 날리자
	Super::Logout(Exiting);
}

void ALobbyGM::CountConnectCount()
{
	//로그아웃 해도 컨트롤러는 살아있으서 제대로 안나온다
	//잘 변경 해보자
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
			//게임 시작
			StartGame();
		}
	}
}

void ALobbyGM::StartGame()
{
	//OpenLevel은 처음에만 사용하고 이후에는 서버에서 맵을 연다
	GetWorld()->ServerTravel(TEXT("Battle"));
}
