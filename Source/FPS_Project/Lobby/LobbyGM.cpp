// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGM::StartGame()
{
	//Game Battle Map Move, Network Map 이동, Travel
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Battle")); -> 이거 안슴
	GetWorld()->ServerTravel(TEXT("Battle"));
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->ConnectCount++;
		GS->OnRep_ConnectCount(); // host 추가 서버 본인 변경.
	}
}

void ALobbyGM::Logout(AController* Exiting)
{
	
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->ConnectCount--;
		GS->OnRep_ConnectCount();  // host 추가 서버 본인 변경.
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
		GS->OnRep_LeftTime();  // host 추가 서버 본인 변경.
	}
}
