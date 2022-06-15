// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton* StartServerButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton* ConnectButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UEditableTextBox* UserID;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UEditableTextBox* Password;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UEditableTextBox* ServerIP;

	UFUNCTION()
	void ClickStartServerButton();

	UFUNCTION()
	void ClickedConnectButton();


};
