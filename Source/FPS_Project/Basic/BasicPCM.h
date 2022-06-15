// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicPCM.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API ABasicPCM : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	virtual void UpdateCamera(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalFOV = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IronsightFOV = 60.f;
};
