// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

struct FSettings;

UCLASS()
class IMPULSE_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	enum ESwitchToWidget
	{
		MainMenu,
		PlayMenu,
		SettingsMenu,
		PracticeMenu
	};

	FSettings RequestSettings();

#pragma region WidgetSwitching

public:
	void SwitchWidget(const ESwitchToWidget WidgetToSwitchTo);

	UFUNCTION()
	void ReturnToPreviousWidget();

protected:
	UUserWidget* CurrentWidget;
	UUserWidget* PreviousWidget;

#pragma endregion 

#pragma region WidgetReferences
	
protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY()
	class UMainMenuWidget* MainMenuWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayMenuClass;

	UPROPERTY()
	class UPlayMenuWidget* PlayMenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SettingsMenuClass;

	UPROPERTY()
	class USettingsMenuWidget* SettingsMenuWidget;

#pragma endregion
};
