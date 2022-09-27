// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/ImpulsePlayerController.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "Character/Components/InWorldUIComponent.h"
#include "GameModes/ImpulseGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/ImpulsePlayerState.h"
#include "Player/Preferences/PlayerPreferences.h"
#include "UI/ImpulseHUD.h"

void AImpulsePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalPlayerController())
	{
		return;
	}
	
	ReadInSettings();

	// TODO: Create chat widget here
	// TODO: Get cheat manager
	HUD = Cast<AImpulseHUD>(GetHUD());

	AvailableChatMethods.Add(Team);
	AvailableChatMethods.Add(Match);
	
	InputComponent->BindAction("Scoreboard", IE_Pressed, HUD, &AImpulseHUD::Scoreboard);
	InputComponent->BindAction("Scoreboard", IE_Released, HUD, &AImpulseHUD::Scoreboard);

	InputComponent->BindAction("InGameMenu", IE_Pressed, HUD, &AImpulseHUD::InGameMenu);

	InputComponent->BindAction("Chat", IE_Pressed, HUD, &AImpulseHUD::Chat);

}

void AImpulsePlayerController::ReadInSettings()
{
	if (UPlayerPreferences* LoadGame = Cast<UPlayerPreferences>(UGameplayStatics::LoadGameFromSlot("PlayerPrefs", 0)))
	{
		Settings.MasterVolume = LoadGame->MasterVolume;
		Settings.MusicVolume = LoadGame->MusicVolume;
		Settings.CharactersVolume = LoadGame->CharactersVolume;
		Settings.AmbientVolume = LoadGame->AmbientVolume;
		Settings.WeaponVolume = LoadGame->WeaponVolume;
		Settings.UIVolume = LoadGame->UIVolume;
		Settings.MouseSensitivity = LoadGame->MouseSensitivity;
		Settings.ZoomSensitivity = LoadGame->ZoomSensitivity;
		Settings.bShowFps = LoadGame->bShowFps;
	}
	

	//PlayerInput->SetMouseSensitivity(Settings.MouseSensitivity);
	// TODO: apply settings after reading them in
}

void AImpulsePlayerController::UpdatePlayerPreferences(const FString& SlotName, const int32 UserIndex, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved player preferences successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save player preferences"));
	}
}

void AImpulsePlayerController::Client_ReceiveChat_Implementation(const FString& SendersName, const FText& Message, EChatTypes TypeOfChat)
{
	HUD->ReceiveChat(SendersName, Message, TypeOfChat);
}

void AImpulsePlayerController::Server_SendChat_Implementation(const FText& Message, EChatTypes TypeOfChat)
{
	if (AImpulseGameStateBase* GameState = Cast<AImpulseGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameState->SendPublicChat(Cast<AImpulsePlayerState>(GetPlayerState<APlayerState>()), Message, TypeOfChat);
	}
}


void AImpulsePlayerController::Server_SetSteamName_Implementation()
{
	Username = GetPlayerState<AImpulsePlayerState>()->GetPlayerName();
	ForceNetUpdate();
}

void AImpulsePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AImpulsePlayerController, Username);
}

void AImpulsePlayerController::HandleDeath()
{
	//PlayerCameraManager->
	// TODO: Set cam to follow killer
}

void AImpulsePlayerController::HandleChangeHealth(float CurrentHealth, float MaxHealth)
{
	HUD->OnHealthChanged(CurrentHealth, MaxHealth);
}

void AImpulsePlayerController::HandleDamageIndicator(AActor* LastActorToDamage, FVector ReceiverLocation)
{
	HUD->OnTakeDamage(LastActorToDamage, ReceiverLocation);
}

void AImpulsePlayerController::HandleChangeAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (HUD)
	{
		HUD->OnAmmoChange(CurrentAmmo, MaxAmmo);
	}
}

void AImpulsePlayerController::Client_UpdateKillFeed_Implementation(const FString& Killer, bool bHeadShot, const FString& Victim)
{
	HUD->UpdateKillFeed(Killer, bHeadShot, Victim);
}

void AImpulsePlayerController::UpdateTimer_Implementation(int32 NewTimeInSeconds)
{
	if (HUD)
	{
		HUD->UpdateTimer(NewTimeInSeconds);
	}
}

void AImpulsePlayerController::OnPlayerHit_Implementation(bool HeadShot, float Damage, AActor* DamagedActor, FVector HitLocation)
{
	if (AImpulseDefaultCharacter* PlayerCharacter = Cast<AImpulseDefaultCharacter>(GetCharacter()))
	{
		if (AImpulseDefaultCharacter* OtherPlayer = Cast<AImpulseDefaultCharacter>(DamagedActor))
		{
			if (AImpulsePlayerState* OtherPlayerState = Cast<AImpulsePlayerState>(OtherPlayer->GetPlayerState()))
			{
				PlayerCharacter->InWorldUIComponent->Client_CreateNameTag(FText::FromString(OtherPlayerState->GetName()), DamagedActor);
			}
		}
		PlayerCharacter->InWorldUIComponent->Client_CreateDamageNumber(Damage, DamagedActor, HitLocation);
		PlayerCharacter->InWorldUIComponent->Client_BeginDestroyNameTag(DamagedActor);
	}
	
	HUD->OnPlayerHit(HeadShot, Damage, DamagedActor);
	if (HeadShot)
	{
		ClientPlaySound(CritSound);
	}
	else
	{
		ClientPlaySound(HitSound);
	}
}

void AImpulsePlayerController::Client_SetControlRotation_Implementation(FRotator NewControlRotation)
{
	SetControlRotation(NewControlRotation);
}

