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
	//Replicated : �����ð����� ������Ų�ٴ� �� -> ��Ʈ��ũ���� ����.
	//ReplicatedUsing="" : ConnectCount�� �ٲ�� ""���� �Լ��� �ڵ� ����ȴ�.
	//�� ƽ���� üũ���� ���� ���� ��ȭ�Ҷ��� �ٲٰ� �Ѵ�.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing="OnRep_ConnectCount", Category = "NetWork")
	int ConnectCount = 0;

	//ConnectCount�� ��ȭ�ϸ� �ڵ� ����Ǵ� �Լ�
	//Ŭ���̾�Ʈ���� �Ǵµ� ������ �ȵȴ� �׷��� ���Ӹ�忡�� Count++��Ű�� �� �Լ� ȣ���ѹ� ����
	UFUNCTION()
		void OnRep_ConnectCount();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_LeftTime", Category = "NetWork")
		int LeftTime = 60;

	UFUNCTION()
		void OnRep_LeftTime();
};
