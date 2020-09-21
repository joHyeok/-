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
		//�÷����ϴ� ��� ���ڸ�
		//���߸�� ����
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
		//�ʱ�ȭ ���ִ� �۾� �ʼ�
		GS->AlivePlayer = 0;

		//���� �÷��̾� ���� ����
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
		{
			ABattlePC* PC = Cast<ABattlePC>(*Iter);
			if (PC)
			{
				ABasicPlayer* Pawn = Cast<ABasicPlayer>(PC->GetPawn());
				if (Pawn)
				{
					//ü���� 0�̸� ���� �ʴ´�
					if (Pawn->CurrentHP > 0)
					{
						GS->AlivePlayer++;
					}
				}
			}
		}
		//Host Only, Update UI
		GS->OnRep_AliveCount();

		//���� ��� ��ŷ
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

		//GetNumPlayers() > 1 �� �����ָ� ������ Battle�� �� ���� �÷��̾�� 1���̶� ����ǹ�����
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
							//1�� ��ŷ ����
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
