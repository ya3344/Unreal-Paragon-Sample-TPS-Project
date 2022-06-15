// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	//Host -> Client 전달, 일정 시간(기본값 0.1 변경 가능)
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)

	// 값이 바뀌면 지정함수 호출
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_ConnectCount")
	int ConnectCount = 0;

	UFUNCTION()
	void OnRep_ConnectCount();

	// 값이 바뀌면 지정함수 호출
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_LeftTime")
	int LeftTime = 60;

	UFUNCTION()
	void OnRep_LeftTime();

	//variable replicate에는 무조건 생성
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
