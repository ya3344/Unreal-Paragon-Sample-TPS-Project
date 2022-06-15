// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTextBlock* ConnectCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTextBlock* PublicMessage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UEditableTextBox* ChatInput;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UButton* StartGame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UScrollBox* ChatBox;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStart();

	UFUNCTION()
	void OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod);

	void HideStartGame();

	void SetConnectCount(int NewConnectCount);

	void SetPublicMessage(int LeftTime);

	void AddMessage(const FString& Message);
};
