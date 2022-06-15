// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "ZombieAIController.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
		, FRotator(0, -90.f, 0));

	Tags.Add(TEXT("Monster"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AZombie::OnSeenPawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &AZombie::OnHeardPawn);
	}
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombie::OnSeenPawn(APawn* Pawn)
{
	if (Pawn->ActorHasTag(TEXT("Player")) && CurrentState == EZombieState::Normal)
	{
		SetCurrentState(EZombieState::Chase);
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			AIC->SetPlayer(Pawn);
		}
	}
}

void AZombie::OnHeardPawn(APawn* NoisePawn, const FVector& Location, float Volume)
{
}

void AZombie::SetCurrentState(EZombieState NewState)
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC)
	{
		CurrentState = NewState; // Pawn Value Update
		AIC->SetCurrentState(NewState); // BehaviorTree-Blackboard key update
	}


}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHP -= DamageAmount;
	CurrentHP = FMath::Clamp(CurrentHP, 0.f, 100.f);

	if (CurrentHP <= 0)
	{
		SetCurrentState(EZombieState::Dead);
	}

	return 0.0f;
}

