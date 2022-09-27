// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ImpulseGameModeBase.generated.h"

/**
 * 
 */
class APlayerSpawn;
class AImpulsePlayerController;
class AImpulseDefaultCharacter;
class AImpulseOnlineBeaconHostObject;
class AOnlineBeaconHost;

UCLASS()
class IMPULSE_API AImpulseGameModeBase : public AGameMode
{
	GENERATED_BODY()

#pragma region Construction

protected:

	AImpulseGameModeBase();
	
	virtual void BeginPlay() override;

	bool CreateHostBeacon();

	AImpulseOnlineBeaconHostObject* BeaconHostObject;

	AOnlineBeaconHost* Host;
	
	AImpulseOnlineBeaconHostObject* GetBeaconHostObject() const { return BeaconHostObject; }
	
#pragma endregion 

#pragma region Spawning
	
	UPROPERTY(EditAnywhere, Category="Classes")
	TSubclassOf<APlayerSpawn> PlayerSpawnerClass;

	// Array of all possible player spawn locations
	TArray<APlayerSpawn*> Spawns;

	// Spawns designated for team A
	TArray<APlayerSpawn*> TeamASpawns; 

	// Spawns designated for team B
	TArray<APlayerSpawn*> TeamBSpawns;

	// Array of all player controllers
	UPROPERTY()
	TArray<APlayerController*> AllPlayerControllers;

	// Set minimum distance a player can spawn from an enemy player
	UPROPERTY(EditDefaultsOnly)
	float MinimumSpawnDistanceFromEnemy = 0;

	// Collects all player spawns and puts them in the Spawns array
	void GetAllPlayerSpawns();

	/**
	 *	Picks a spawn location for a player to spawn at given the parameters
	 *	@param SpawnsToChooseFrom The array of spawns that this player can spawn from
	 *	@param Team the team that this player is on
	 */
	TArray<APlayerSpawn*> ChooseSpawn(TArray<APlayerSpawn*> SpawnsToChooseFrom, uint8 Team);

	/**
	 *	Gives the player control of its newly spawned character
	 *	@param PlayerController The player's controller that will receive possession
	 *	@param ControlRotation The direction the player should spawn looking at
	 *	@param PawnToPossess The new pawn that the player should possess
	 */
	void PossessSpawnedCharacter(APlayerController* PlayerController, FRotator ControlRotation, AImpulseDefaultCharacter* PawnToPossess);

public:

	// Checks if spawns are separate
	bool AreSpawnsSeparate() const { return bSeparateSpawns; }

	// Determines if spawns are separate. False by default
	bool bSeparateSpawns = false;

	
	void Server_ResetSpawns();
	void SpawnCharacter(APlayerController* PlayerController);

#pragma endregion
	
#pragma region Settings
	// Can this game have bots
	bool bAllowBots;

	// checks game options like and bots
	void CheckGameOptions();
	void SetGameWithBots(bool AllowBots);
	
	TArray<APlayerController*> GetAllPlayers() { return AllPlayerControllers; }
	
#pragma endregion

#pragma region Log in/out
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	
#pragma endregion

#pragma region Gameplay

private:
	bool bGameOver;

protected:
	virtual void HandleMatchHasEnded() override;
	
public:
	bool IsGameOver() const { return bGameOver; }

#pragma endregion 
};

