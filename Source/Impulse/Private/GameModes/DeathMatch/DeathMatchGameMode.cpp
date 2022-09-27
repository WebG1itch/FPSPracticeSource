// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DeathMatch/DeathMatchGameMode.h"

void ADeathMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	MinimumSpawnDistanceFromEnemy = 1000.f;
}

