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
	//Host -> Client ����, ���� �ð�(�⺻�� 0.1 ���� ����)
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)

	// ���� �ٲ�� �����Լ� ȣ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_ConnectCount")
	int ConnectCount = 0;

	UFUNCTION()
	void OnRep_ConnectCount();

	// ���� �ٲ�� �����Լ� ȣ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_LeftTime")
	int LeftTime = 60;

	UFUNCTION()
	void OnRep_LeftTime();

	//variable replicate���� ������ ����
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
