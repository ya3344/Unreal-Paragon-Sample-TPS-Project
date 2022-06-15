// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "TitleWidgetBase.h"

void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	// ���� ��Ʈ�ѿ����� ������ ����. �̷��� �ؾ� �������� �ѹ��� TitleWidget ����.
	if (IsLocalPlayerController() && TitleWidgetClass)
	{
		bShowMouseCursor = true; // ���콺 Ŀ�� ǥ��

		SetInputMode(FInputModeUIOnly()); // Ű����, ���콺 �Է��� UI���� ����.

		// instance ����
		TitleWidget = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);

		// �������� UI ȭ�鿡 �߰�.
		TitleWidget->AddToViewport();
	}
}
