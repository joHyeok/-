// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGameInstance.h"

FString UBDGameInstance::GetUserID()
{
	//UserID�� ���ٸ� �ϳ� �����ؼ� ��ȯ�Ѵ�
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
