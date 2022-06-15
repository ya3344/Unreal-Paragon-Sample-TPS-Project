// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void AZombieAIController::SetSpeedValue()
{
	AZombie* Zombie = Cast<AZombie>(GetPawn());
	if (Zombie)
	{
		BBComponent->SetValueAsFloat(TEXT("WalkSpeed"), Zombie->WalkSpeed);
		BBComponent->SetValueAsFloat(TEXT("RunSpeed"), Zombie->RunSpeed);
	}
}

void AZombieAIController::SetPlayer(AActor* Player)
{
	if (Player)
	{
		BBComponent->SetValueAsObject(TEXT("Player"), Player);
	}
}

void AZombieAIController::SetCurrentState(EZombieState CurrentState)
{
	BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)CurrentState);
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogClass, Warning, TEXT("Zombie Onpossess"));

	AZombie* Zombie = Cast<AZombie>(InPawn);
	if (Zombie)
	{
		BBComponent->InitializeBlackboard(*(Zombie->ZombieBT->BlackboardAsset));
		BTComponent->StartTree(*(Zombie->ZombieBT));

		BBComponent->SetValueAsVector(TEXT("Target"), FVector(0, 0, 0));

		SetSpeedValue();
		SetCurrentState(EZombieState::Normal);
	}
}

void AZombieAIController::OnUnPossess()
{
	BTComponent->StopTree();
	Super::OnUnPossess();
}

