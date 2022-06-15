// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "../BDGameInstance.h"

void UTitleWidgetBase::NativeConstruct()
{
	StartServerButton = Cast<UButton>(GetWidgetFromName(TEXT("StartServerButton")));
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("Connect")));
	UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserID")));
	Password = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Password"))); 
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));

	if (StartServerButton)
	{
		StartServerButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ClickStartServerButton);
	}

	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ClickedConnectButton);
	}


}

void UTitleWidgetBase::ClickStartServerButton()
{
	UBDGameInstance* BDGI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (BDGI)
	{
		BDGI->SetUserID(UserID->GetText().ToString());
	}

	//GetplayerController()->MakeServer(); playercontroller 에서 하는게 구조가 좋음.
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen")); // 서버로 시작
}

void UTitleWidgetBase::ClickedConnectButton()
{
	UBDGameInstance* BDGI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (BDGI)
	{
		BDGI->SetUserID(UserID->GetText().ToString());
	}


	if (ServerIP && !ServerIP->GetText().IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerIP->GetText().ToString()));
	}
}
