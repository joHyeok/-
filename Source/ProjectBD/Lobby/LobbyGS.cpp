// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/Unrealnetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "LobbyWidgetBase.h"

void ALobbyGS::OnRep_ConnectCount()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//LobbyWidgetObject가 있을때만 실행시키자
	//안그러면 위젯이 생성되지도 않았는데 바꿀려고 한다.
	if (PC && PC->LobbyWidgetObject)
	{
		PC->LobbyWidgetObject->SetConnectCount(ConnectCount);
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//자동으로 네트워크에서 동작하게 해주는 코드
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
