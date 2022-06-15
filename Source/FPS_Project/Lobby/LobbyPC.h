// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPC();

	//BlueprintClass, Widget Blueprint Class
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
	TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;

	//Blueprint Instance, Widget Blueprint Instance
	class ULobbyWidgetBase* LobbyWidget;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// Ŭ���̾�Ʈ ȣ���ϰԵǸ� Server���� ���� 
	UFUNCTION(Server, Reliable, WithValidation) // Unreal �⺻������ UDP �̱� ������ Reliable ����
	// Not implementation Call
	void C2S_SendMessage(const FString& Message); // network ������ & pointer�� �����ؾ� �Ѵ�. �����Ϸ� üũ�Ͽ� Call by value ����(UHT)
	// �������� Validate �������� ���� ��  Implementation ����
	bool C2S_SendMessage_Validate(const FString& Message);
	// Excute
	void C2S_SendMessage_Implementation(const FString& Message);

	UFUNCTION(Client, Reliable)
	// not implementation
	void S2C_AddMessage(const FString& Message); 
	// client�̹Ƿ� ���� �ʿ����
	//bool S2C_SendMessage_Validate(const FString& Message); 
	 // implementation Execute
	void S2C_AddMessage_Implementation(const FString& Message);


};
