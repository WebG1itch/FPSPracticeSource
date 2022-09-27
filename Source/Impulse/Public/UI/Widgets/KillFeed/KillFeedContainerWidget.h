// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillFeedContainerWidget.generated.h"

class UScrollBox;

UCLASS()
class IMPULSE_API UKillFeedContainerWidget : public UUserWidget
{
	GENERATED_BODY()
	UKillFeedContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SB_KillFeed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> KillFeedInstanceWidgetClass;
	
	UPROPERTY()
	class UKillFeedInstanceWidget* KillFeedInstanceWidget;


	void HandleKillFeedOverflow();
	
public:
	void UpdateKillFeed(const FString& KillerName, bool bHeadShot, const FString& VictimName);
};
