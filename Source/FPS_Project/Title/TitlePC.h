// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePC.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API ATitlePC : public APlayerController
{
	GENERATED_BODY()
public:
	//Widget Class
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UTitleWidgetBase> TitleWidgetClass;

	//Widget Instance
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTitleWidgetBase* TitleWidget;

	virtual void BeginPlay() override;
};
