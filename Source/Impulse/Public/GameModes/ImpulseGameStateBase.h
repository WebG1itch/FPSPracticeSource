// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Enums/EChatTypes.h"
#include "ImpulseGameStateBase.generated.h"


class AImpulseGameModeBase;
class AImpulsePlayerState;

UCLASS()
class IMPULSE_API AImpulseGameStateBase : public AGameState
{
	GENERATED_BODY()

	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	AImpulseGameModeBase* ImpulseGameModeBase;

	TArray<class AImpulsePlayerController*> Players;

#pragma region Timers

protected:
	float RespawnTime;

	UPROPERTY(Replicated)
	float TimeLeftToStartMatch;

	UPROPERTY(Replicated)
	float GameTimeInSeconds;

	UPROPERTY(Replicated)
	float TimeLeftForNextSpawn;
	
	FTimerHandle UpdateCountDownTimerHandle;
	FTimerHandle MatchTimerHandle;
	FTimerHandle TimeLeftToSpawnHandle;

	void UpdateMatchTimer();

#pragma endregion

#pragma region Gameplay

public:

	/**
	*	Called by the dead player's state when its character dies. By default it updates the kill feed and lets the instigating player's state know it got a kill
	*	@param DeadPlayerController The controller of the player that died
	*	@param Killer The Killer's PlayerState
	*	@param Victim The Victim's PlayerState
	*	@param WasHeadShot Whether or not the kill was a HeadShot. False by default
	*/	
	UFUNCTION(Server, Reliable)
	virtual void OnPlayerDeath(APlayerController* DeadPlayerController, AImpulsePlayerState* Killer, AImpulsePlayerState* Victim, bool WasHeadShot = false);

	/**
	 *	Called by the chat widget when a user want's to send a message to the server. Supports team and match chat.
	 *	@param SendersState the player state of the sender
	 *	@param Message the message to be sent
	 */	
	UFUNCTION(Server, Reliable)
	void SendPublicChat(AImpulsePlayerState* SendersState, const FText& Message, EChatTypes TypeOfChat);
	
protected:

	/**
	 * Initiates the respawn for a dead player.
	 * @param PlayerController Controller of the player that needs respawned
	 */
	UFUNCTION(Server, Reliable)
	virtual void RespawnPlayer(APlayerController* PlayerController);
	
public:
	/**
	 * Called by the game mode when a new player joins the match and adds the player to the list of player controllers
	 * @param NewPlayer Controller of the player that just joined
	 */
	virtual void AddPlayer(AController* NewPlayer);

protected:
	/**
	 * tells game mode to transition to in progress and starts a match timer
	 */
	virtual void StartMatch();

	// TODO: Move to a mode with objectives
	void GetObjectives();

#pragma endregion 

	// Built in - properties to be replicated.
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;
};
 /* before line buffing: line count 77 */