// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "TitleWidgetBase.h"

void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	// 로컬 컨트롤에서만 위젯을 생성. 이렇게 해야 서버에서 한번만 TitleWidget 생성.
	if (IsLocalPlayerController() && TitleWidgetClass)
	{
		bShowMouseCursor = true; // 마우스 커서 표시

		SetInputMode(FInputModeUIOnly()); // 키보드, 마우스 입력을 UI에만 전달.

		// instance 생성
		TitleWidget = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);

		// 셀로판지 UI 화면에 추가.
		TitleWidget->AddToViewport();
	}
}
