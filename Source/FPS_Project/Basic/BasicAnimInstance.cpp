// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAnimInstance.h"
#include "BasicPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABasicPlayer* Pawn = Cast<ABasicPlayer>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity, Pawn->GetActorRotation());
		
		bIsCrouch = Pawn->bIsCrouched;
		bIsIronsight = Pawn->bIsIronsight;

		FRotator Aim = Pawn->GetAimOffset();
		AimYaw = Aim.Yaw;
		AimPitch = Aim.Pitch;

		//FireAlpha = Pawn->bIsFire ? 1.0f : 0.f;
	}
}
