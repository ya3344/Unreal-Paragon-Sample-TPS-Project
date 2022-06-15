// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGM.h"
#include "LobbyPC.h"
#include "../BDGameInstance.h"

void ULobbyWidgetBase::NativeConstruct()
{
	ConnectCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ConnectCount")));
	PublicMessage = Cast<UTextBlock>(GetWidgetFromName(TEXT("PublicMessage")));
	StartGame = Cast<UButton>(GetWidgetFromName(TEXT("StartGame")));
	ChatInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatInput")));
	ChatBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatBox")));

	if (StartGame)
	{
		StartGame->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OnStart);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnTextCommit);
	}
}

void ULobbyWidgetBase::OnStart()
{
	//Excute Logic Use Not
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GM) // Server?
	{
		GM->StartGame();
	}
}

void ULobbyWidgetBase::OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			//widet은 palyercontroller 밑에 있기때문에 GetOwingPlayer 로 가능
			ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer());
		/*	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(),
				0));*/

			UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (PC && GI)
			{
				FString Message = FString::Printf(TEXT("%s : %s"), *GI->UserID, *Text.ToString());
				PC->C2S_SendMessage(Message); // Server Send
				ChatInput->SetText(FText::FromString(TEXT("")));
			}
		}
		break;

		case ETextCommit::OnCleared:
		{
			ChatInput->SetUserFocus(GetOwningPlayer());
		}
		break;
	}
}

void ULobbyWidgetBase::HideStartGame()
{
	StartGame->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidgetBase::SetConnectCount(int NewConnectCount)
{
	if (ConnectCount)
	{
		FString Temp = FString::Printf(TEXT("%d"), NewConnectCount);
		ConnectCount->SetText(FText::FromString(Temp));
	}

}

void ULobbyWidgetBase::SetPublicMessage(int LeftTime)
{
	if (LeftTime)
	{
		FString Temp = FString::Printf(TEXT("%d초 후에 시작합니다."), LeftTime);
		PublicMessage->SetText(FText::FromString(Temp));
	}

}

void ULobbyWidgetBase::AddMessage(const FString& Message)
{
	if (ChatBox)
	{
		//SpawnActor,
		//SpawnEmitter,
		//SpawnSound
		//CreateWidget
		//나를 누가만들었는지 ChatBox 밑에 들어가므로 ChatBox 가 Owner
		UTextBlock* NewMessage = NewObject<UTextBlock>(ChatBox);
		if (NewMessage)
		{
			NewMessage->SetText(FText::FromString(Message));
			NewMessage->Font.Size = 18;
			ChatBox->AddChild(NewMessage);
			ChatBox->ScrollToEnd();
		}

		// 지우는것은 자기 스스로 개발 필요
	}
}
