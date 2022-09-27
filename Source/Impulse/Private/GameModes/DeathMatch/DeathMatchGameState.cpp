// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DeathMatch/DeathMatchGameState.h"

#include "GameModes/ImpulseGameModeBase.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "PlayerState/ImpulsePlayerState.h"

void ADeathMatchGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ADeathMatchGameState::OnPlayerDeath(APlayerController* DeadPlayerController, AImpulsePlayerState* Killer,
                                         AImpulsePlayerState* Victim, bool WasHeadShot)
{
	Super::OnPlayerDeath(DeadPlayerController, Killer, Victim, WasHeadShot);

	if (Killer->GetKillCount() >= KillsNeededToWin && ImpulseGameModeBase->IsMatchInProgress())
	{
		ImpulseGameModeBase->EndMatch();
	}
	
	if (AImpulsePlayerController* DeadPlayersController = Cast<AImpulsePlayerController>(DeadPlayerController))
	{
		FTimerHandle RespawnTimerHandle;
		FTimerDelegate TimerDelegate;

		TimerDelegate.BindUFunction(this, FName("RespawnPlayer"), DeadPlayersController);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, TimerDelegate, 5.0f, false);
	}
}

void ADeathMatchGameState::WarmUpTimer()
{
	WarmUpTime -= 1;
	
	for (APlayerController* Player : Players)
	{
		if (AImpulsePlayerController* PlayerController = Cast<AImpulsePlayerController>(Player))
		{
			PlayerController->UpdateTimer(WarmUpTime);
		}
	}
	
	if (WarmUpTime <= 0)
	{
		StartMatch();

		if (GetWorldTimerManager().TimerExists(WarmUpTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(WarmUpTimerHandle);
		}
		
		return;
	}
	
	
	GetWorldTimerManager().SetTimer(WarmUpTimerHandle, this, &ADeathMatchGameState::WarmUpTimer, 1.0f);
}

void ADeathMatchGameState::AddPlayer(AController* NewPlayer)
{
	if (Players.IsEmpty())
	{
		WarmUpTimer();
	}
		
	Super::AddPlayer(NewPlayer);

	NewPlayer->GetPlayerState<AImpulsePlayerState>()->Team = Players.Num();

	if (Players.Num() > 8 && WarmUpTime > 40)
	{
		WarmUpTime = 30;
	}
}
