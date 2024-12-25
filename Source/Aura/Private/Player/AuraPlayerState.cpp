// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;	// when GAS is part of PlayerState make sure to set to higher update rate than default
}