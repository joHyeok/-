// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UEditableTextBox* ChatInput;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UScrollBox* ChatBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTextBlock* ConnectCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTextBlock* PublicMessage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UButton* GameStartButton;

	UFUNCTION()
	void ProcessTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	void AddMessage(FText Message);

	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetConnectCount(int NewConnectCount);

	void SetPublicMessage(int LeftTime);

	UFUNCTION()
	void PressStartGameButton();

	void HideStartGameButton();

};
