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

	//클라이언트에서 서버로 메세지 보내기
	//C2S_SendMessage를 실행하면 서버를 타고 가서 C2S_SendMessage_Validate를 실행하고 false면
	//실행시키지 않는다 true라면 C2S_SendMessage_Implementation를 실행한다
	//구현은 C2S_SendMessage_Implementation에서
	
	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(const FText& Message);
	bool C2S_SendMessage_Validate(const FText& Message);
	void C2S_SendMessage_Implementation(const FText& Message);

	UFUNCTION(Client, Reliable)
	void S2C_SendMessage(const FText& Message);
	void S2C_SendMessage_Implementation(const FText& Message);
};
