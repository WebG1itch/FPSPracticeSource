// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/ImpulsePlayerState.h"

#include "GameModes/ImpulseGameModeBase.h"
#include "Kismet/GameplayStatics.h"	
#include "Net/UnrealNetwork.h"

void AImpulsePlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GameMode = Cast<AImpulseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsABot())
		{
			SetBotName();
		}
	}
	Name = GetPlayerName();
	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Name);
}



void AImpulsePlayerState::Server_SetTeam_Implementation(uint8 NewTeam)
{
	Team = NewTeam;
	
	ForceNetUpdate();
}

void AImpulsePlayerState::SetBotName()
{
	Name = TEXT("Bot");
}

void AImpulsePlayerState::Client_NeedsUpdate_Implementation()
{
	bNeedsUpdate = true;
}

void AImpulsePlayerState::OnSuicide()
{
	if (!GameMode->IsGameOver())
	{
		Kills -= 1;
		ForceNetUpdate();
	}
}

void AImpulsePlayerState::AddKill()
{
	if (!GameMode->IsGameOver())
	{
		Kills += 1;
		ForceNetUpdate();
	}
}

void AImpulsePlayerState::HandleDeath()
{
	Deaths += 1;
	bDead = true;
}

void AImpulsePlayerState::HandleRespawn()
{
	bDead = false;
}

void AImpulsePlayerState::ResetScore()
{
	Kills = 0;
	Deaths = 0;
	Assists = 0;
}

FString AImpulsePlayerState::GetName()
{
	if (IsABot())
	{
		return Name;
	}

	return GetPlayerName();
}

void AImpulsePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AImpulsePlayerState, Name);
	DOREPLIFETIME(AImpulsePlayerState, bDead);
	DOREPLIFETIME(AImpulsePlayerState, Kills);
	DOREPLIFETIME(AImpulsePlayerState, Team);
	DOREPLIFETIME(AImpulsePlayerState, Deaths);
	DOREPLIFETIME(AImpulsePlayerState, Assists);
}

