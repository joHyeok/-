// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPC();

	virtual void BeginPlay()override;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	//	TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
		class ULobbyWidgetBase* LobbyWidgetObject;

	//Ŭ���̾�Ʈ���� ������ �޼��� ������
	//C2S_SendMessage�� �����ϸ� ������ Ÿ�� ���� C2S_SendMessage_Validate�� �����ϰ� false��
	//�����Ű�� �ʴ´� true��� C2S_SendMessage_Implementation�� �����Ѵ�
	//������ C2S_SendMessage_Implementation����
	
	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(const FText& Message);
	bool C2S_SendMessage_Validate(const FText& Message);
	void C2S_SendMessage_Implementation(const FText& Message);

	UFUNCTION(Client, Reliable)
	void S2C_SendMessage(const FText& Message);
	void S2C_SendMessage_Implementation(const FText& Message);
};
