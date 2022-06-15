// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckSpeed.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API UBTService_CheckSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
