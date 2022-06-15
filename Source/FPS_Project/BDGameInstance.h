// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API UBDGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UserID;

	UFUNCTION(BlueprintCallable)
	void SetUserID(FString NewUserID);
};
