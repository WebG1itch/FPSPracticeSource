// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ImpulseGameModeBase.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "GameModes/ImpulseGameInstance.h"
#include "GameModes/ImpulseGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "PlayerSpawn/PlayerSpawn.h"
#include "PlayerState/ImpulsePlayerState.h"
#include "Networking/ImpulseGameSession.h"
#include "OnlineBeaconHost.h"
#include "Networking/Beacons/ImpulseOnlineBeaconHostObject.h"

#pragma region Construction

AImpulseGameModeBase::AImpulseGameModeBase()
{
	BeaconHostObject = nullptr;
	Host = nullptr;
	// TODO: Set default pawn class to the blueprint version of the character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Character"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AImpulseGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GetAllPlayerSpawns();
	CheckGameOptions();

	GameSession->RegisterServer();
	CreateHostBeacon();
	// TODO: Set difficulty of bots
	// TODO: Manage number of bots
}

bool AImpulseGameModeBase::CreateHostBeacon()
{
	Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass());
	
	if (Host)
	{
		if (Host->InitHost())
		{
			Host->PauseBeaconRequests(false);

			BeaconHostObject = GetWorld()->SpawnActor<AImpulseOnlineBeaconHostObject>(AImpulseOnlineBeaconHostObject::StaticClass());
			if (BeaconHostObject)
			{
				Host->RegisterHost(BeaconHostObject);
				return true;
			}
			
		}
	}
	return false;
}

#pragma endregion 

#pragma region Spawning

void AImpulseGameModeBase::GetAllPlayerSpawns()
{
	TArray<AActor*> OutSpawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerSpawnerClass, OutSpawns);
	for (AActor* SpawnActor : OutSpawns)
	{
		APlayerSpawn* Spawn = Cast<APlayerSpawn>(SpawnActor);
		Spawns.Add(Spawn);
		
		if (bSeparateSpawns)
		{
			Spawn->Team == 0 ? TeamASpawns.Add(Spawn) : TeamBSpawns.Add(Spawn);
		}
	}
}
 
TArray<APlayerSpawn*> AImpulseGameModeBase::ChooseSpawn(TArray<APlayerSpawn*> SpawnsToChooseFrom, uint8 Team)
{
	TArray<APlayerSpawn*> BestPossibleSpawns;
	BestPossibleSpawns.Empty();
	
	for (APlayerSpawn* Spawn : SpawnsToChooseFrom)
	{
		APlayerSpawn* ActualSpawn = Spawn;
		Spawn->bNearEnemy = false;
		for (APlayerController* Player : AllPlayerControllers)
		{
			if (Player)
			{
				AImpulsePlayerState* PlayersState = Cast<AImpulsePlayerState>(Player->PlayerState);
				if (!PlayersState->IsDead() && (!bSeparateSpawns || Team != PlayersState->Team))
				{
					if (Player->GetPawn())
					{
						ActualSpawn->bNearEnemy = FMath::Abs((Player->GetPawn()->GetActorLocation() - ActualSpawn->GetActorLocation()).Length()) <= MinimumSpawnDistanceFromEnemy;
					}
				}
			}
		}

		if (!ActualSpawn->bNearEnemy)
		{
			BestPossibleSpawns.Add(ActualSpawn);
		}
	}

	if (BestPossibleSpawns.Num() > 0)
	{
		return BestPossibleSpawns;
	}

	return SpawnsToChooseFrom;
}

void AImpulseGameModeBase::PossessSpawnedCharacter(APlayerController* PlayerController, const FRotator ControlRotation, AImpulseDefaultCharacter* PawnToPossess)
{
	PlayerController->Possess(PawnToPossess);
	PlayerController->SetControlRotation(ControlRotation);
}

void AImpulseGameModeBase::Server_ResetSpawns()
{
	if (!bSeparateSpawns)
	{
		for(APlayerSpawn* SpawnActor : Spawns)
		{
			SpawnActor->bCanBeUsed = true;
		}
	}
	else
	{
		for (APlayerSpawn* SpawnActor : TeamASpawns)
		{
			SpawnActor->bCanBeUsed = true;
		}
		
		for (APlayerSpawn* SpawnActor : TeamBSpawns)
		{
			SpawnActor->bCanBeUsed = true;
		}
	}
}

void AImpulseGameModeBase::SpawnCharacter(APlayerController* PlayerController)
{
	
	if(PlayerController->GetPawn())
	{
		PlayerController->GetPawn()->Destroy();		
	}
	
	TArray<APlayerSpawn*> BestPossibleSpawns;
	
	APlayerSpawn* SelectedSpawn;
	
	AImpulsePlayerState* PlayerState = Cast<AImpulsePlayerState>(PlayerController->PlayerState);

	if (bSeparateSpawns)
	{
		BestPossibleSpawns = ChooseSpawn(PlayerState->Team == 0 ? TeamASpawns : TeamBSpawns, PlayerState->Team);
	}
	else
	{
		BestPossibleSpawns = ChooseSpawn(Spawns, PlayerState->Team);
	}

	if (BestPossibleSpawns[0])
	{
		SelectedSpawn = BestPossibleSpawns[FMath::RandRange(0, BestPossibleSpawns.Num() - 1)];
		SelectedSpawn->bCanBeUsed = false;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerController;

		AImpulseDefaultCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AImpulseDefaultCharacter>(DefaultPawnClass, SelectedSpawn->GetActorLocation(), SelectedSpawn->GetActorRotation(), SpawnParams);
		PossessSpawnedCharacter(PlayerController, SpawnedCharacter->GetActorRotation(), SpawnedCharacter);

		PlayerState->HandleRespawn();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Failed. Check AImpulseGameModeBase::SpawnCharacter"));
	}
}


#pragma endregion 

#pragma region Settings

void AImpulseGameModeBase::CheckGameOptions()
{
	const UImpulseGameInstance* GameInstance = Cast<UImpulseGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return;
	}
	
	SetGameWithBots(GameInstance->bAllowBots);
}

void AImpulseGameModeBase::SetGameWithBots(const bool AllowBots)
{
	bAllowBots = AllowBots;
}

#pragma endregion

#pragma region Log in/out

void AImpulseGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AImpulseGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	if (APlayerController* NewPlayerController = Cast<AImpulsePlayerController>(NewPlayer))
	{
		AllPlayerControllers.Add(NewPlayerController);
	}

	AImpulseGameStateBase* ImpulseGameState = GetGameState<AImpulseGameStateBase>();
	ImpulseGameState->AddPlayer(NewPlayer);
}

void AImpulseGameModeBase::HandleMatchHasEnded()
{
	bGameOver = true;
	Super::HandleMatchHasEnded();
	
	// TODO: tell all clients to go back to the main menu
}



#pragma endregion 
