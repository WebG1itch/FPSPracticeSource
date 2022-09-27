// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/EChatTypes.h"
#include "GameFramework/HUD.h"
#include "Widgets/DamageIndicatorWidget.h"
#include "ImpulseHUD.generated.h"

class UUserWidget;

USTRUCT()
struct FDamageIndicatorInfo
{
	GENERATED_BODY()
	
	UDamageIndicatorWidget* WidgetBeingUsed = nullptr;
	AActor* ActorWhoInstigated = nullptr;
	FTimerHandle TimerUntilRemoved;
};

UCLASS()
class IMPULSE_API AImpulseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AImpulseHUD();
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GamePlayWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CustomCrosshairClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DamageCounterClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DamageIndicatorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DeathMatchScoreboardClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> KillFeedContainerWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChatBoxClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SettingsMenuClass;
	
	UPROPERTY()
	class UCustomCrosshairWidget* CustomCrosshairWidget;

	UPROPERTY()
	class UGamePlayWidget* GamePlayWidget;

	UPROPERTY()
	class UDamageCounterWidget* DamageCounterWidget;

	UPROPERTY()
	class UDeathMatchWidget* DeathMatchWidget;

	UPROPERTY()
	class UKillFeedContainerWidget* KillFeedContainerWidget;

	UPROPERTY()
	class UPauseMenu* PauseMenuWidget;

	UPROPERTY()
    class UChatBox* ChatBoxWidget;

	UPROPERTY()
	class USettingsMenuWidget* SettingsMenuWidget;
	
	FTimerHandle DamageCounterTimerHandle;
	
	virtual void BeginPlay() override;

	void DestroyDamageCounter();
public:
	void OnAmmoChange(int32 CurrentAmmo, int32 MaxAmmo);

	void UpdateKillFeed(const FString& Killer, bool bHeadShot, const FString& Victim);

	UFUNCTION()
	void InGameMenu();

	UFUNCTION()
	void Chat();

	void Scoreboard();

	UFUNCTION()
	void ReturnToPauseMenu();

	UFUNCTION()
	void OpenSettings();

	UFUNCTION()
	void OnHealthChanged(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void BackToMenu();

	void ReceiveChat(FString SendersName, const FText& Message, EChatTypes TypeOfChat);

	void OnPlayerHit(bool HeadShot, float Damage, AActor* DamagedActor);
	void UpdateTimer(int32 NewTimeInSeconds);
	
	void OnTakeDamage(AActor* InstigatingActor, FVector ReceiverLocation);

	TArray<FDamageIndicatorInfo> RecentDamagers;
};
