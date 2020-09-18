// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	//Replicated : 일정시간마다 복제시킨다는 뜻 -> 네트워크에서 쓴다.
	//ReplicatedUsing="" : ConnectCount가 바뀌면 ""안의 함수가 자동 실행된다.
	//매 틱마다 체크하지 말고 값이 변화할때만 바꾸게 한다.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing="OnRep_ConnectCount", Category = "NetWork")
	int ConnectCount = 0;

	//ConnectCount가 변화하면 자동 실행되는 함수
	//클라이언트들은 되는데 서버만 안된다 그래서 게임모드에서 Count++시키고 이 함수 호출한번 하자
	UFUNCTION()
		void OnRep_ConnectCount();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_LeftTime", Category = "NetWork")
		int LeftTime = 60;

	UFUNCTION()
		void OnRep_LeftTime();
};
