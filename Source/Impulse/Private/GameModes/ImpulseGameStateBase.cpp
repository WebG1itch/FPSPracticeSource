// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ImpulseGameStateBase.h"
#include "GameModes/ImpulseGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "PlayerState/ImpulsePlayerState.h"


void AImpulseGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		GetObjectives(); //TODO: Move to objective specific game modes
		ImpulseGameModeBase = Cast<AImpulseGameModeBase>(GetWorld()->GetAuthGameMode<AImpulseGameModeBase>());
	}
}

#pragma region Timers


void AImpulseGameStateBase::UpdateMatchTimer()
{
	GameTimeInSeconds += 1;
	if (GetWorldTimerManager().TimerExists(TimeLeftToSpawnHandle))
	{
		TimeLeftForNextSpawn = GetWorldTimerManager().GetTimerRemaining(TimeLeftToSpawnHandle);
	}
	
	for (APlayerController* Player : Players)
	{
		if (AImpulsePlayerController* PlayerController = Cast<AImpulsePlayerController>(Player))
		{
			PlayerController->UpdateTimer(FMath::RoundToInt32(GameTimeInSeconds));
		}
	}
		
	ForceNetUpdate();
}

void AImpulseGameStateBase::SendPublicChat_Implementation(AImpulsePlayerState* SendersState, const FText& Message, EChatTypes TypeOfChat)
{
	for (AImpulsePlayerController* PlayerController : Players)
	{
		switch (TypeOfChat)
		{
			case Match:
			{
				if (PlayerController->GetPlayerState<AImpulsePlayerState>()->Team != SendersState->Team)
				{
					UE_LOG(LogTemp, Warning, TEXT("send to enemy"));

					PlayerController->Client_ReceiveChat(SendersState->GetName(), Message, Enemy);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("send to mate"));
					
					PlayerController->Client_ReceiveChat(SendersState->GetName(), Message, Team);
				}
				break;
			}
			case Team:
			{
				if (PlayerController->GetPlayerState<AImpulsePlayerState>()->Team == SendersState->Team)
				{
					PlayerController->Client_ReceiveChat(SendersState->GetName(), Message, Team);
				}
				break;
			}
			default:
			{
				
				if (PlayerController->GetPlayerState<AImpulsePlayerState>()->Team != SendersState->Team)
				{
					PlayerController->Client_ReceiveChat(SendersState->GetName(), Message, Enemy);
				}
				else
				{
					PlayerController->Client_ReceiveChat(SendersState->GetName(), Message, Team);
				}
			}
		}
	}
}

void AImpulseGameStateBase::RespawnPlayer_Implementation(APlayerController* PlayerController)
{
	if (ImpulseGameModeBase)
	{
		ImpulseGameModeBase->SpawnCharacter(PlayerController);
	}
}

void AImpulseGameStateBase::OnPlayerDeath_Implementation(APlayerController* DeadPlayerController, AImpulsePlayerState* Killer, AImpulsePlayerState* Victim, bool WasHeadShot)
{
	if (AImpulsePlayerController* DeadPlayersController = Cast<AImpulsePlayerController>(DeadPlayerController))
	{
		DeadPlayersController->HandleDeath();
	}

	// Check if suicide
	Killer == Victim ? Victim->OnSuicide() : Killer->AddKill();
	
	// Tell all players they need to update
	for (APlayerState* PlayerState : PlayerArray)
	{
		AImpulsePlayerState* Player = Cast<AImpulsePlayerState>(PlayerState);
		Player->Client_NeedsUpdate_Implementation();
	}

	// Update player kill feeds
	for (AImpulsePlayerController* Player : Players)
	{
		if(Player)
		{
			Player->Client_UpdateKillFeed(Killer->GetName(), WasHeadShot, Victim->GetName());
		}
	}
}

#pragma endregion 

void AImpulseGameStateBase::AddPlayer(AController* NewPlayer)
{
	if (AImpulsePlayerController* ImpulsePlayer = Cast<AImpulsePlayerController>(NewPlayer))
	{
		Players.Add(ImpulsePlayer);
	}

	for (APlayerState* PlayerState : PlayerArray)
	{
		AImpulsePlayerState* Player = Cast<AImpulsePlayerState>(PlayerState);
		Player->Client_NeedsUpdate_Implementation();
	}
}

void AImpulseGameStateBase::StartMatch()
{
	// Clear pre-match timers
	GetWorldTimerManager().ClearTimer(UpdateCountDownTimerHandle);

	// Start match and set new timers
	ImpulseGameModeBase->StartMatch();
	
	GetWorldTimerManager().SetTimer(MatchTimerHandle, this, &AImpulseGameStateBase::UpdateMatchTimer, 1.0f, true);
	//GetWorldTimerManager().SetTimer(TimeLeftToSpawnHandle, this, &AImpulseGameStateBase::CheckElapsedTime, RespawnTime, true);
	//CheckElapsedTime();
	
	// TODO: Create bots on GameMode

	// Update all players when match is started.
	for (APlayerState* PlayerState : PlayerArray)
	{
		AImpulsePlayerState* Player = Cast<AImpulsePlayerState>(PlayerState);
		Player->Client_NeedsUpdate_Implementation();
	}
}

void AImpulseGameStateBase::GetObjectives()
{
	// TODO: Get all objectives on map
}

void AImpulseGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AImpulseGameStateBase, TimeLeftToStartMatch);
	DOREPLIFETIME(AImpulseGameStateBase, GameTimeInSeconds);
	DOREPLIFETIME(AImpulseGameStateBase, TimeLeftForNextSpawn);
}
