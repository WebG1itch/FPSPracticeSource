// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ImpulsePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API AImpulsePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
#pragma region Variables

	UPROPERTY(Replicated)
	FString Name;

	UPROPERTY(Replicated)
	bool bDead;

	UPROPERTY(Replicated)
	int32 Kills;

	UPROPERTY(Replicated)
	int32 Deaths;

	UPROPERTY(Replicated)
	int32 Assists;
	
	class AImpulseGameModeBase* GameMode;

	UFUNCTION(Server, Reliable)
	void Server_SetTeam(uint8 NewTeam);

	void SetBotName();

	UFUNCTION(Client, Reliable)
	void Client_NeedsUpdate();

public:
	bool bNeedsUpdate = true;

	void OnSuicide();
	void AddKill();
	void HandleDeath();
	void HandleRespawn();
	void ResetScore();
	bool IsDead() const { return bDead; }
	
	int32 GetKillCount() const { return Kills; }
	int32 GetDeathCount() const { return Deaths; }
	int32 GetAssistCount() const { return Assists; }
	
	FString GetName();

	UPROPERTY(Replicated)
	uint8 Team;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion
};
