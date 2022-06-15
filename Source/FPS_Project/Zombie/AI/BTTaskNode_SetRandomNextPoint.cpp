// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetRandomNextPoint.h"
#include "../Zombie.h"
#include "../ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTaskNode_SetRandomNextPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC && AIC->GetPawn())
	{
		FVector NewTarget = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(),
			AIC->GetPawn()->GetActorLocation(),
			500.f);

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("Target"), NewTarget);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
