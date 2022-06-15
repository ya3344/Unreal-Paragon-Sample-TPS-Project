// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "Engine/World.h"
#include "LobbyWidgetBase.h"

void ALobbyGS::OnRep_ConnectCount()
{
	// 0�� PC�̸� �ڱ� �ڽ� Local PlayerController
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->SetConnectCount(ConnectCount);
	}
}

void ALobbyGS::OnRep_LeftTime()
{
	// 0�� PC�̸� �ڱ� �ڽ� Local PlayerController
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->SetPublicMessage(LeftTime);
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicated ���� �߰� //������ �༭ ������ ��ũ�ε� ���� ����
	DOREPLIFETIME(ALobbyGS, ConnectCount);
	DOREPLIFETIME(ALobbyGS, LeftTime);
	
}
