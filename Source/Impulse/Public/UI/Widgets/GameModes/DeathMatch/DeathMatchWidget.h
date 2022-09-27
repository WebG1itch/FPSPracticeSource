// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathMatchWidget.generated.h"

class AImpulsePlayerState;
class AImpulsePlayerController;
class AImpulseGameStateBase;
class UVerticalBox;
UCLASS()
class IMPULSE_API UDeathMatchWidget : public UUserWidget
{
	GENERATED_BODY()
	UDeathMatchWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_PlayerScores;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerScoreWidgetClass;
	
	UPROPERTY()
	class UDMPlayerScoreWidget* PlayerScoreWidget;
	
	virtual void NativeConstruct() override;

	AImpulseGameStateBase* GameState;
	AImpulsePlayerController* OwningPlayerController;
	AImpulsePlayerState* OwningPlayerState;

	TArray<AImpulsePlayerState*> Players;
	
	void CreateLists();
	void UpdateLists();
	void UpdateScoreboard();
};
