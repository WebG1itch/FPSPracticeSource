// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillFeedInstanceWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class IMPULSE_API UKillFeedInstanceWidget : public UUserWidget
{
	GENERATED_BODY()
	UKillFeedInstanceWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Killer;

	UPROPERTY(meta = (BindWidget))
	UImage* I_Image;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_DeadPlayer;

	virtual void NativeConstruct() override;

	void SelfDestruct();
public:
	void SetVariables(FString KillerName, bool bHeadShot, FString VictimName);
};
