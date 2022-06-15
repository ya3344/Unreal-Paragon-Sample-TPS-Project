// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.12f; // �Ⱓ
	OscillationBlendInTime = 0.05f; // ���� ���� �ð�
	OscillationBlendOutTime = 0.05f; // �� ���� �ð�

	RotOscillation.Pitch.Amplitude = 40.f; // ���� ����
	RotOscillation.Pitch.Frequency = 1.f; // Ƚ��(Duration)
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero; // ���۰�(0)
}
