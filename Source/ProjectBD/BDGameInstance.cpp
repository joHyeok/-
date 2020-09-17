// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGameInstance.h"

FString UBDGameInstance::GetUserID()
{
	//UserID가 없다면 하나 생성해서 반환한다
	if (UserID.IsEmpty())
	{
		FString Temp = FString::Printf(TEXT("Noname%d"), FMath::RandRange(1, 100));
		UserID = Temp;
	}

	return UserID;
}

void UBDGameInstance::SetUserID(FString NewUserID)
{
	UserID = NewUserID;
}
