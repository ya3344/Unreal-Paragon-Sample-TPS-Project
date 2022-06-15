// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Zombie.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
    EZombieState CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float Speed;

    UFUNCTION()
    void AnimNotify_CheckAttack(UAnimNotify* Notify);
};
