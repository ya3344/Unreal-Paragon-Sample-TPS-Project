// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API UBasicAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	uint32 bIsCrouch : 1;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	bool bIsIronsight;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	float AimYaw;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	float AimPitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	float FireAlpha;
};
