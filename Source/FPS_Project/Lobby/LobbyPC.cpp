// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGS.h"

ALobbyPC::ALobbyPC()
{
	//PrimaryActorTick.bCanEverTick = true; // �� �����Ӹ��� ƽ�� ���ư����� true�� ���� default false
}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());

		//���� ��� ����, blueprint �ǰ�� _c �ٿ��� ��.
		FSoftClassPath LobbyWidgetbasePath(TEXT("WidgetBlueprint'/Game/Blueprints/Lobby/UI/LobbyWidget.LobbyWidget_C'"));
	
		//#inlcude
		UClass* LobbyWidgetLoadClass = LobbyWidgetbasePath.TryLoadClass<ULobbyWidgetBase>();
		if (LobbyWidgetLoadClass)
		{
			//new like c++
			LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetLoadClass);
			if (LobbyWidget)
			{
				LobbyWidget->AddToViewport();
			}

			if (!HasAuthority()) // Not Server
			{
				//LobbyWidget StartButton Hide
				LobbyWidget->HideStartGame();
			}
		}

	}
}

void ALobbyPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));

	if (GS && IsLocalPlayerController())
	{
		LobbyWidget->SetConnectCount(GS->ConnectCount);
	}
}

bool ALobbyPC::C2S_SendMessage_Validate(const FString& Message)
{
	// return false �� ��Ʈ��ũ �������� �����ϰ� ��Ʈ��ũ ����.
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(const FString& Message)
{
	//all PC->AddMessage
	for (auto iter = GetWorld()->GetPlayerControllerIterator(); iter; iter++)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*iter);
		if (PC)
		{
			PC->S2C_AddMessage(Message);
		}
	}
}

void ALobbyPC::S2C_AddMessage_Implementation(const FString& Message)
{
	if (LobbyWidget)
	{
		LobbyWidget->AddMessage(Message);
	}
}

//void ALobbyPC::SendMessage(const FString& Message)
//{
//}
