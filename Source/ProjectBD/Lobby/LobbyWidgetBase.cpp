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
		//�ؽ�Ʈ�� �����ϴ� ��. ���͸� ������ �Էµȴ�.
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
		//Local PC -> Host PC ȣ���� ���ÿ���, ������ ȣ��Ʈ����

		ALobbyPC* PC = GetOwningPlayer<ALobbyPC>();
		if (PC)
		{
			UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI)
			{
				FString Temp = FString::Printf(TEXT("%s : %s"), *GI->GetUserID(), *Text.ToString());

				//���͸� ������ ������ ä���ƴٰ� �˸���
				PC->C2S_SendMessage(FText::FromString(Temp));

				//ChatInputâ�� �� ���ڵ� �����
				ChatInput->SetText(FText::FromString(TEXT("")));
			}
		}
	}
		break;
	case ETextCommit::OnUserMovedFocus:
		break;
	case ETextCommit::OnCleared:
	{
		//ChatInput�� ����� ��Ŀ���� �������
		//�׷��� ��Ŀ���� �÷��̾���Ʈ�ѷ��� �����ش�
		//�׷� �ٽ� ��Ŀ���� ��������
		ChatInput->SetUserFocus(GetOwningPlayer());
	}
		break;
	}
}

void ULobbyWidgetBase::AddMessage(FText Message)
{
	if (ChatBox)
	{
		//�ؽ�Ʈ ��� ���� ����
		//(��� �������)
		UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatBox);

		if (NewTextBlock)
		{
			//���������� �ؽ�Ʈ�� �������� ������ �ؽ�Ʈ�� �Է��Ѵ�.
			NewTextBlock->SetText(Message);
			NewTextBlock->Font.Size = 18;
			//�ڽı��� ����
			ChatBox->AddChild(NewTextBlock);
			//������ �������� ä���� ġ�� �ֽ� ä���� ���;��ϹǷ� ���� ������ ���������Ѵ�
			ChatBox->ScrollToEnd();
		}
	}
}
