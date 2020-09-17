// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "LobbyPC.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "../BDGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ChatInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatInput")));

	if (ChatInput)
	{
		//텍스트를 인지하는 것. 엔터를 누르면 입력된다.
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::ProcessTextCommited);
	}

	ChatBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatBox")));
}

void ULobbyWidgetBase::ProcessTextCommited(const FText & Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::Default:
		break;
	case ETextCommit::OnEnter:
	{
		//Local PC -> Host PC 호출은 로컬에서, 실행은 호스트에서

		ALobbyPC* PC = GetOwningPlayer<ALobbyPC>();
		if (PC)
		{
			UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI)
			{
				FString Temp = FString::Printf(TEXT("%s : %s"), *GI->GetUserID(), *Text.ToString());

				//엔터를 누르면 서버로 채팅쳤다고 알린다
				PC->C2S_SendMessage(FText::FromString(Temp));

				//ChatInput창에 쓴 문자들 지우기
				ChatInput->SetText(FText::FromString(TEXT("")));
			}
		}
	}
		break;
	case ETextCommit::OnUserMovedFocus:
		break;
	case ETextCommit::OnCleared:
	{
		//ChatInput을 지우면 포커스가 사라진다
		//그래서 포커스를 플레이어컨트롤러로 맞춰준다
		//그럼 다시 포커스가 맞춰진다
		ChatInput->SetUserFocus(GetOwningPlayer());
	}
		break;
	}
}

void ULobbyWidgetBase::AddMessage(FText Message)
{
	if (ChatBox)
	{
		//텍스트 블록 동적 생성
		//(어디에 만들건지)
		UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatBox);

		if (NewTextBlock)
		{
			//동적생성한 텍스트에 서버에서 보내온 텍스트를 입력한다.
			NewTextBlock->SetText(Message);
			NewTextBlock->Font.Size = 18;
			//자식구조 설정
			ChatBox->AddChild(NewTextBlock);
			//밑으로 내려가라 채팅을 치면 최신 채팅이 나와야하므로 가장 밑으로 내려가야한다
			ChatBox->ScrollToEnd();
		}
	}
}
