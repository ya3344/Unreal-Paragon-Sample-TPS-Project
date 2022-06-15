// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Zombie.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	AZombieAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBlackboardComponent* BBComponent;

	void SetSpeedValue();
	void SetPlayer(AActor* Player);

	void SetCurrentState(EZombieState CurrentState);
};
