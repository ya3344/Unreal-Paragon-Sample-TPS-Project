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

	// 접속 완료 후 PlayerController 생성 후 실행.
	virtual void PostLogin(APlayerController* NewPlayer) override;

	//로그아웃 후 PlayerController 삭제전
	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	FTimerHandle HandleLeftTime;

	UFUNCTION()
	void LeftTime();
};
