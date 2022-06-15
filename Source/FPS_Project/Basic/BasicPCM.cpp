// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPCM.h"
#include "BasicPC.h"
#include "BasicPlayer.h"

void ABasicPCM::UpdateCamera(float DeltaTime)
{
	//PlayerController
	ABasicPC* PC = Cast<ABasicPC>(GetOwningPlayerController());

	if (PC)
	{
		//Player
		ABasicPlayer* Pawn = Cast<ABasicPlayer>(PC->GetPawn());
		if (Pawn)
		{
			//Ironsight FOV
			float TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
			SetFOV(FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 15.f));

			//Crouched Camera height
			FVector TargetSpringArmPosition = Pawn->bIsCrouched ? Pawn->CrouchedSpringArmPosition :
				Pawn->NormalSpringArmPosition;

			Pawn->SetSpringArmPosition(FMath::VInterpTo(Pawn->GetSpringArmPosition(),
				TargetSpringArmPosition, DeltaTime, 5.f));
		}
	}

	Super::UpdateCamera(DeltaTime);
}
