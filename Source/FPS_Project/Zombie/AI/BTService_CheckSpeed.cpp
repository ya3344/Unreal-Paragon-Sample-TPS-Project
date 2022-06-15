// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckSpeed.h"
#include "../Zombie.h"
#include "../ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBTService_CheckSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie)
	{
		if (Zombie->CurrentState == EZombieState::Chase)
		{
			Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->RunSpeed;
		}
	}
}
