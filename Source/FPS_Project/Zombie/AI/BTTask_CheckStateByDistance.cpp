// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckStateByDistance.h"
#include "../ZombieAIController.h"
#include "BehaviorTree//BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CheckStateByDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));

	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Zombie || !Player)
	{
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());

	switch (Condition)
	{
		case ETaskCondition::Less:
		{
			if (Distance < Range)
			{
				Zombie->SetCurrentState(TargetState);
			}
		}
		break;

		case ETaskCondition::Greater:
		{
			if (Distance > Range)
			{
				Zombie->SetCurrentState(TargetState);
			}
		}
		break;
	}


	return EBTNodeResult::Succeeded;
}
