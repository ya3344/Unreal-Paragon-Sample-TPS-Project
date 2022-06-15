// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Engine/StreamableManager.h"


void ABasicHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Crosshair)
	{
		DrawTextureSimple(Crosshair, Canvas->SizeX / 2 - (Crosshair->GetSizeX()/2), 
			Canvas->SizeY / 2 - (Crosshair->GetSizeY() / 2));
	}
}

void ABasicHUD::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager Loader;

	FSoftObjectPath CrossHairPath(TEXT("Texture2D'/Game/Data/Crosshair/crosshair.crosshair'"));

	Loader.LoadSynchronous<UTexture2D>(CrossHairPath);

	Crosshair = Loader.LoadSynchronous<UTexture2D>(CrossHairPath);

}
