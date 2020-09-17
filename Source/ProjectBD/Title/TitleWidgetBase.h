// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UMG Widget과 Control 변수 연결
	virtual void NativeConstruct()override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UEditableTextBox* UserID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
		class UEditableTextBox* Passward;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
		class UEditableTextBox* ServerIP;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
		class UButton* ConnectButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
		class UButton* StartButton;

	UFUNCTION()
		void OnConnectButton();

	UFUNCTION()
		void OnStartButton();

	void SaveUserID();
};
