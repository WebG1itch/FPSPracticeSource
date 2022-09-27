// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ImpulseGameModeBase.h"
#include "DeathMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API ADeathMatchGameMode : public AImpulseGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
