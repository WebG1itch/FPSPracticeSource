// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "SettingsMenuWidget.generated.h"

class UButton;
class UVerticalBox;
class UWidgetSwitcher;
class AMainMenuHUD;
class UPlayerPreferences;
class UCheckBox;
class USlider;
class UCanvasPanel;

UCLASS()
class IMPULSE_API USettingsMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	const int PlayMenuIndex = 1;
	const int SettingsMenuIndex = 2;
	
	USettingsMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	virtual void NativeConstruct() override;

	UPROPERTY()
	AHUD* HUDReference;

	bool bIsMainMenu;

#pragma region Load/Save Preferenced
	
	void LoadPlayerPreferences(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	UFUNCTION()
	void SavePlayerPreferences();


	UPROPERTY()
	UPlayerPreferences* PlayerPreferences;

	FAsyncSaveGameToSlotDelegate SavedDelegate;

#pragma endregion
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_ApplySettings;


	UFUNCTION()
	void ApplySettings() { CP_ApplySettingsPrompt->SetVisibility(ESlateVisibility::Visible); }

	UPROPERTY()
	UCanvasPanel* CP_ApplySettingsPrompt;

	UPROPERTY(meta = (BindWidget))
	UButton* B_ApplySettingsForReal;

	UPROPERTY(meta = (BindWidget))
	UButton* B_CancelApplySettings;

	UFUNCTION()
	void CancelApplySettings() { CP_ApplySettingsPrompt->SetVisibility(ESlateVisibility::Hidden); }
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Back;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_GameplayTab;
	
	UFUNCTION()
	void OnGameplayTabReleased() { SettingsSwitcher->SetActiveWidgetIndex(0); }
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_KeybindsTab;

	UFUNCTION()
	void OnKeybindsTabReleased() { SettingsSwitcher->SetActiveWidgetIndex(1); }
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_VideoTab;

	UFUNCTION()
	void OnVideoTabReleased() { SettingsSwitcher->SetActiveWidgetIndex(2); }
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_AudioTab;

	UFUNCTION()
	void OnAudioTabReleased() { SettingsSwitcher->SetActiveWidgetIndex(3); }
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_GameplayTab;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_AudioTab;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_VideoTab;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* SettingsSwitcher;


#pragma region Gameplay

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CB_ToggleSprint;

	bool bToggleSprint;

#pragma endregion

#pragma region MouseAndKeyboard

#pragma endregion

#pragma region Video

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CB_VSync;

	bool bUseVsync;

#pragma endregion

#pragma region Audio

	UPROPERTY(meta = (BindWidget))
	USlider* S_MasterVolume;

	int MasterVolume;

	UPROPERTY(meta = (BindWidget))
	USlider* S_SFXVolume;

	int SFXVolume;
	
	UPROPERTY(meta = (BindWidget))
	USlider* S_MusicVolume;

	int MusicVolume;

#pragma endregion 
	
	
public:
	void SetHUDReference(AHUD* MainMenuReference, bool IsMainMenu);
};
