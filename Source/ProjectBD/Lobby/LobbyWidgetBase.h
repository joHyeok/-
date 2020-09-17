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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Widget")
		class UEditableTextBox* ChatInput;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Widget")
		class UScrollBox* ChatBox;

	UFUNCTION()
	void ProcessTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	void AddMessage(FText Message);
};
