// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DMPlayerScoreWidget.generated.h"

class UTextBlock;
class UHorizontalBox;

UCLASS()
class IMPULSE_API UDMPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	UDMPlayerScoreWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_Box;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_PlayerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Kills;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Deaths;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Assists;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Ping;
	
public:
	void UpdatePlayerScoreInfo(FString Name, int32 KillCount, int32 DeathCount, int32 AssistCount, int32 Ping, bool bIsSelf);
};
