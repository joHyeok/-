// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGM.h"
#include "BattleGS.h"
#include "BattlePC.h"
#include "BattlePS.h"
#include "../Basic/BasicPlayer.h"

void ABattleGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		//플레이하는 사람 숫자만
		//관중모드 빼고
		GS->TotalPlayerCount = GetNumPlayers();
	}

	CountAlivePlayer();
}

void ABattleGM::Logout(AController * Exiting)
{
	CountAlivePlayer();
	Super::Logout(Exiting);
}

void ABattleGM::CountAlivePlayer()
{
	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		//초기화 해주는 작업 필수
		GS->AlivePlayer = 0;

		//남은 플레이어 숫자 세기
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
		{
			ABattlePC* PC = Cast<ABattlePC>(*Iter);
			if (PC)
			{
				ABasicPlayer* Pawn = Cast<ABasicPlayer>(PC->GetPawn());
				if (Pawn)
				{
					//체력이 0이면 세지 않는다
					if (Pawn->CurrentHP > 0)
					{
						GS->AlivePlayer++;
					}
				}
			}
		}
		//Host Only, Update UI
		GS->OnRep_AliveCount();

		//죽은 사람 랭킹
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
		{
			ABattlePC* PC = Cast<ABattlePC>(*Iter);
			if (PC)
			{
				ABasicPlayer* Pawn = Cast<ABasicPlayer>(PC->GetPawn());
				if (Pawn)
				{
					if (Pawn->CurrentHP <= 0)
					{
						ABattlePS* PS = Pawn->GetPlayerState<ABattlePS>();
						if (PS)
						{
							if (PS->PlayerRank == -1)
							{
								PS->PlayerRank = GS->AlivePlayer + 1;
							}
						}
					}
				}
			}
		}

		//GetNumPlayers() > 1 를 안해주면 방장이 Battle에 들어간 순간 플레이어는 1명이라서 실행되버린다
		if (GS->AlivePlayer == 1 && GetNumPlayers() > 1)
		{
			//GameOver
			UE_LOG(LogClass, Warning, TEXT("GameOver"));

			for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
			{
				ABattlePC* PC = Cast<ABattlePC>(*Iter);
				if (PC)
				{
					ABasicPlayer* Pawn = Cast<ABasicPlayer>(PC->GetPawn());
					if (Pawn)
					{
						if (Pawn->CurrentHP > 0)
						{
							//1등 랭킹 지정
							ABattlePS* PS = Pawn->GetPlayerState<ABattlePS>();
							PS->PlayerRank = 1;
						}
					}
				}
			}

			//GameOver
			//Network Result UI Show
			//Lobby Timer
			FTimerHandle EndingTimer;
			GetWorldTimerManager().SetTimer(EndingTimer, this, &ABattleGM::GoLobby, 3.0f);
		}
	}
}

void ABattleGM::GoLobby()
{
	GetWorld()->ServerTravel(TEXT("Lobby"));
}

void ABattleGM::DecreaseCountPlayer()
{
	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		GS->AlivePlayer--;
		GS->OnRep_AliveCount();
	}
}

void ABattleGM::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
}
