// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGameInstance.h"

void UBDGameInstance::SetUserID(FString NewUserID)
{
	if (NewUserID.IsEmpty())
	{
		UserID = NewUserID;
	}
	else
	{
		UserID = FString::Printf(TEXT("NoName%d"), FMath::RandRange(0, 100));
	}
	
}
