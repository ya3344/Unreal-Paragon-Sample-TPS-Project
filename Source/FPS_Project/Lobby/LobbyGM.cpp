// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGM::StartGame()
{
	//Game Battle Map Move, Network Map �̵�, Travel
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Battle")); -> �̰� �Ƚ�
	GetWorld()->ServerTravel(TEXT("Battle"));
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->ConnectCount++;
		GS->OnRep_ConnectCount(); // host �߰� ���� ���� ����.
	}
}

void ALobbyGM::Logout(AController* Exiting)
{
	
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->ConnectCount--;
		GS->OnRep_ConnectCount();  // host �߰� ���� ���� ����.
	}
	Super::Logout(Exiting);
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		HandleLeftTime,
		this,
		&ALobbyGM::LeftTime,
		1.f,
		true,
		1.f
	);
}

void ALobbyGM::LeftTime()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->LeftTime--;
		GS->OnRep_LeftTime();  // host �߰� ���� ���� ����.
	}
}
