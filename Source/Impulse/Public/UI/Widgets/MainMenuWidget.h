// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UEditableText;
class UButton;
class UCanvasPanel;
class UVerticalBox;
class UWidgetSwitcher;
class AMainMenuHUD;
class UTextBlock;

UCLASS()
class IMPULSE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	virtual void NativeConstruct() override;

	//void BackToPreviousIndex();

	//void MoveToNextWidgetIndex(int Index);

	// UPROPERTY(meta = (BindWidget))
	// UWidgetSwitcher* WidgetSwitcher;

	int8 CurrentWidgetIndex = 0;
	int8 PreviousWidgetIndex = 0;

	// index of main menu widget
	const int MainMenuIndex = 0;
	// index of play menu widget
	const int PlayMenuIndex = 1;
	// index of settings menu widget
	const int SettingsMenuIndex = 2;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CP_MainMenu;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_ExitGame;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Practice;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Settings;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Play;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Name;
	
	UFUNCTION()
	void OnExitGameButtonReleased();

	UFUNCTION()
	void OnPlayButtonReleased();

	UFUNCTION()
	void OnSettingsButtonReleased();

	UPROPERTY()
	AMainMenuHUD* HUDReference;
	
public:
	void SetHUDReference(AMainMenuHUD* MainMenuReference);
	
};
