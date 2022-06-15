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

	// 클라이언트 호출하게되면 Server에서 실행 
	UFUNCTION(Server, Reliable, WithValidation) // Unreal 기본적으로 UDP 이기 떄문에 Reliable 선택
	// Not implementation Call
	void C2S_SendMessage(const FString& Message); // network 전송은 & pointer로 전송해야 한다. 컴파일러 체크하여 Call by value 방지(UHT)
	// 서버에서 Validate 실행으로 검증 후  Implementation 실행
	bool C2S_SendMessage_Validate(const FString& Message);
	// Excute
	void C2S_SendMessage_Implementation(const FString& Message);

	UFUNCTION(Client, Reliable)
	// not implementation
	void S2C_AddMessage(const FString& Message); 
	// client이므로 검증 필요없음
	//bool S2C_SendMessage_Validate(const FString& Message); 
	 // implementation Execute
	void S2C_AddMessage_Implementation(const FString& Message);


};
