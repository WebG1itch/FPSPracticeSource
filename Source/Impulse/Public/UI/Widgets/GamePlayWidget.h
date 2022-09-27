// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;

UCLASS()
class IMPULSE_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()

	UGamePlayWidget(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_Health;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_CurrentAmmo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_MaxAmmo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_HealthAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Timer;
	
	UPROPERTY(meta = (BindWidget))
	UImage* I_HitMarker;

	UPROPERTY(meta = (BindWidget))
	UImage* I_CritMarker;

	UFUNCTION()
	void FadeObject(UWidget* WidgetToChange, const float AmountToChangeBy, const float InRate);

public:
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo) const;
	void UpdateHealth(float CurrentHealth, float MaxHealth) const;
	void OnPlayerHit(bool HeadShot);
	void UpdateTimer(int32 NewTimeInSeconds) const;
};

