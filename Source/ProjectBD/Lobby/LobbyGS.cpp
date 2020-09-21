// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/Unrealnetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "LobbyWidgetBase.h"

void ALobbyGS::OnRep_ConnectCount()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//LobbyWidgetObject�� �������� �����Ű��
	//�ȱ׷��� ������ ���������� �ʾҴµ� �ٲܷ��� �Ѵ�.
	if (PC && PC->LobbyWidgetObject)
	{
		PC->LobbyWidgetObject->SetConnectCount(ConnectCount);
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//�ڵ����� ��Ʈ��ũ���� �����ϰ� ���ִ� �ڵ�
	DOREPLIFETIME(ALobbyGS, ConnectCount);
	DOREPLIFETIME(ALobbyGS, LeftTime);
}

void ALobbyGS::OnRep_LeftTime()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidgetObject)
	{
		PC->LobbyWidgetObject->SetPublicMessage(LeftTime);
	}
}
