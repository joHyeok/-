// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPoint.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"

#if WITH_EDITORONLY_DATA

AItemPoint::AItemPoint()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Icon(TEXT("Texture2D'/Game/Items/Thumb/Medipack.Medipack'"));
	if (T_Icon.Succeeded())
	{
		GetSpriteComponent()->SetSprite(T_Icon.Object);
		//GetSpriteComponent()->SetEditorScale(3.0f);
		NewIcon = T_Icon.Object;
	}
}
#endif