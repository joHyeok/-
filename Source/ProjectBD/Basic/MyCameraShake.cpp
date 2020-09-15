// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	RotOscillation.Pitch.Amplitude = 1.0f;
	RotOscillation.Pitch.Frequency = 1.0;

	OscillationDuration = 0.1f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;
}
