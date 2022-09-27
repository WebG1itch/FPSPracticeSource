// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerPreferences.generated.h"

UCLASS()
class IMPULSE_API UPlayerPreferences : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, Category="Gameplay")
    bool bToggleSprint;
	
	UPROPERTY(VisibleAnywhere, Category="Video")
	bool bUseVSync;
	
	UPROPERTY(VisibleAnywhere, Category="Video")
	int FPSLimit;
    	
	UPROPERTY(VisibleAnywhere, Category="Audio")
	float MasterVolume;
	
	UPROPERTY(VisibleAnywhere, Category="Audio")
	float MusicVolume;

	UPROPERTY(VisibleAnywhere, Category="Audio")
	float CharactersVolume;

	UPROPERTY(VisibleAnywhere, Category="Audio")
	float AmbientVolume;

	UPROPERTY(VisibleAnywhere, Category="Audio")
	float WeaponVolume;
	
	UPROPERTY(VisibleAnywhere, Category="Audio")
	float UIVolume;

	UPROPERTY(VisibleAnywhere, Category="Mouse")
	float MouseSensitivity;

	UPROPERTY(VisibleAnywhere, Category="Mouse")
	float ZoomSensitivity;

	UPROPERTY(VisibleAnywhere, Category="Stats")
	bool bShowFps;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	uint32 UserIndex;

	UPlayerPreferences();
};
