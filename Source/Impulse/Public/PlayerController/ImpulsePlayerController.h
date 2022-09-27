// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/EChatTypes.h"
#include "GameFramework/PlayerController.h"
#include "ImpulsePlayerController.generated.h"

class AImpulseHUD;

struct FSettings
{
	float MasterVolume;
	float MusicVolume;
	float CharactersVolume;
	float AmbientVolume;
	float WeaponVolume;
	float UIVolume;
	float MouseSensitivity;
	float ZoomSensitivity;
	bool bShowFps;
};

UCLASS()
class IMPULSE_API AImpulsePlayerController : public APlayerController
{
	GENERATED_BODY()
		
	virtual void BeginPlay() override;
	
	void ReadInSettings();

public:
	UFUNCTION()
	void UpdatePlayerPreferences(const FString& SlotName, const int32 UserIndex, bool Success);

	UFUNCTION(Server, Reliable)
	void Server_SendChat(const FText& Message, EChatTypes TypeOfChat);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveChat(const FString& Name, const FText& Message, EChatTypes TypeOfChat);

protected:
	UFUNCTION(Server, Reliable)
	void Server_SetSteamName();


	UPROPERTY(Replicated)
	FString Username;

	FSettings Settings;
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;
	
	UPROPERTY()
	AImpulseHUD* HUD;
	
	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	USoundBase* CritSound;
public:

	FSettings GetSettings() const { return Settings; }
	
	UFUNCTION(Client, Reliable)
	void Client_SetControlRotation(FRotator NewControlRotation);

	void HandleDeath();
	void HandleChangeHealth(float CurrentHealth, float MaxHealth);
	void HandleDamageIndicator(AActor* LastActorToDamage, FVector ReceiverLocation);

	void HandleChangeAmmo(int32 CurrentAmmo, int32 MaxAmmo);

	UFUNCTION(Client, Unreliable)
	void Client_UpdateKillFeed(const FString& Killer, bool bHeadShot, const FString& Victim);

	UFUNCTION(Client, Unreliable)
	void UpdateTimer(int32 NewTimeInSeconds);

	UFUNCTION(Client, Reliable)
	void OnPlayerHit(bool HeadShot, float Damage, AActor* DamagedActor, FVector HitLocation);

	TArray<EChatTypes> AvailableChatMethods;

};
