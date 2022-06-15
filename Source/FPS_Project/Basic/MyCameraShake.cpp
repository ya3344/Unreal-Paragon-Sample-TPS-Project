// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.12f; // 기간
	OscillationBlendInTime = 0.05f; // 시작 블렌딩 시간
	OscillationBlendOutTime = 0.05f; // 끝 블렌딩 시간

	RotOscillation.Pitch.Amplitude = 40.f; // 세로 강도
	RotOscillation.Pitch.Frequency = 1.f; // 횟수(Duration)
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero; // 시작값(0)
}
