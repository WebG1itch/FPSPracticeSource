// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ImpulseGameStateBase.h"
#include "DeathMatchGameState.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API ADeathMatchGameState : public AImpulseGameStateBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override; 

	virtual void OnPlayerDeath(APlayerController* DeadPlayerController, AImpulsePlayerState* Killer, AImpulsePlayerState* Victim, bool WasHeadShot) override;

	UPROPERTY(EditDefaultsOnly)
	int8 KillsNeededToWin = 40;

	UPROPERTY(EditDefaultsOnly)
	int8 WarmUpTime = 100;

	FTimerHandle WarmUpTimerHandle;
	
	/**
	 * If timer has not begun, warm up will be started. Otherwise keeps the warm up timer in an adjustable time loop
	 */
	void WarmUpTimer();
	
	virtual void AddPlayer(AController* NewPlayer) override;
	
};
