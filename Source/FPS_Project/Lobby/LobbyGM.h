// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGM.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API ALobbyGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	void StartGame();

	// ���� �Ϸ� �� PlayerController ���� �� ����.
	virtual void PostLogin(APlayerController* NewPlayer) override;

	//�α׾ƿ� �� PlayerController ������
	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	FTimerHandle HandleLeftTime;

	UFUNCTION()
	void LeftTime();
};
