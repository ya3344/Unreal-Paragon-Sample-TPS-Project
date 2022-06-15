// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "Engine/World.h"
#include "LobbyWidgetBase.h"

void ALobbyGS::OnRep_ConnectCount()
{
	// 0번 PC이면 자기 자신 Local PlayerController
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->SetConnectCount(ConnectCount);
	}
}

void ALobbyGS::OnRep_LeftTime()
{
	// 0번 PC이면 자기 자신 Local PlayerController
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->SetPublicMessage(LeftTime);
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicated 변수 추가 //조건을 줘서 보내는 매크로도 있음 참고
	DOREPLIFETIME(ALobbyGS, ConnectCount);
	DOREPLIFETIME(ALobbyGS, LeftTime);
	
}
