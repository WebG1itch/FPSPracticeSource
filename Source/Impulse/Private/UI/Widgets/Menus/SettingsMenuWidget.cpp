// Copyright Spitfire Labs 2022


#include "UI/Widgets/Menus/SettingsMenuWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Player/Preferences/PlayerPreferences.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "UI/ImpulseHUD.h"
#include "UI/MainMenuHUD.h"


void USettingsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (bIsMainMenu)
	{
		AMainMenuHUD* MainMenuReference = Cast<AMainMenuHUD>(HUDReference);
		B_Back->OnReleased.AddDynamic(MainMenuReference, &AMainMenuHUD::ReturnToPreviousWidget);
	}
	else
	{
		AImpulseHUD* GameReference = Cast<AImpulseHUD>(HUDReference);
		B_Back->OnReleased.AddDynamic(GameReference, &AImpulseHUD::ReturnToPauseMenu);
	}
	
	B_GameplayTab->OnReleased.AddDynamic(this, &USettingsMenuWidget::OnGameplayTabReleased);
	B_KeybindsTab->OnReleased.AddDynamic(this, &USettingsMenuWidget::OnKeybindsTabReleased);
	B_VideoTab->OnReleased.AddDynamic(this, &USettingsMenuWidget::OnVideoTabReleased);
	B_AudioTab->OnReleased.AddDynamic(this, &USettingsMenuWidget::OnAudioTabReleased);
	B_ApplySettings->OnReleased.AddDynamic(this, &USettingsMenuWidget::ApplySettings);
	B_ApplySettingsForReal->OnReleased.AddDynamic(this, &USettingsMenuWidget::SavePlayerPreferences);
	B_CancelApplySettings->OnReleased.AddDynamic(this, &USettingsMenuWidget::CancelApplySettings);
	
	SettingsSwitcher->SetActiveWidgetIndex(0);
	
	S_MasterVolume->SetMaxValue(100);
	S_MasterVolume->SetMinValue(0);

	S_SFXVolume->SetMaxValue(100);
	S_SFXVolume->SetMinValue(0);

	S_MusicVolume->SetMaxValue(100);
	S_MusicVolume->SetMinValue(0);
	
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &USettingsMenuWidget::LoadPlayerPreferences);
	UGameplayStatics::AsyncLoadGameFromSlot(FString("PlayerPreferences"), 0, LoadedDelegate);
}

void USettingsMenuWidget::LoadPlayerPreferences(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	UE_LOG(LogTemp, Warning, TEXT("Player preferences locaded"));
	if (UPlayerPreferences* LoadedPreferences = Cast<UPlayerPreferences>(LoadedGameData))
	{
		/* Gameplay Section */
		bToggleSprint = LoadedPreferences->bToggleSprint;
		CB_ToggleSprint->SetCheckedState(bToggleSprint ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		
		/* Video Section*/
		bUseVsync = LoadedPreferences->bUseVSync;
		CB_VSync->SetCheckedState(bUseVsync ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

		/* Audio Section*/
		S_MasterVolume->SetValue(MasterVolume);
		
		S_SFXVolume->SetValue(SFXVolume);

		S_MusicVolume->SetValue(MusicVolume);
	}
}

void USettingsMenuWidget::SavePlayerPreferences()
{
	PlayerPreferences = Cast<UPlayerPreferences>(UGameplayStatics::CreateSaveGameObject(UPlayerPreferences::StaticClass()));
	if (PlayerPreferences)
	{
		if (AImpulsePlayerController* OwningController = Cast<AImpulsePlayerController>(HUDReference->GetOwningPlayerController()))
		{
			SavedDelegate.BindUObject(OwningController, &AImpulsePlayerController::UpdatePlayerPreferences);

			PlayerPreferences->bShowFps = CB_ToggleSprint->IsChecked();
			PlayerPreferences->bUseVSync = CB_VSync->IsChecked();
			PlayerPreferences->MasterVolume = S_MasterVolume->GetValue();
			PlayerPreferences->UIVolume = S_SFXVolume->GetValue();
			PlayerPreferences->MusicVolume = S_MusicVolume->GetValue();

			UGameplayStatics::AsyncSaveGameToSlot(PlayerPreferences, FString("PlayerPreferences"), 0, SavedDelegate);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying settings failed: couldn't load settings file"));
	}

	CP_ApplySettingsPrompt->SetVisibility(ESlateVisibility::Hidden);
}


void USettingsMenuWidget::SetHUDReference(AHUD* MainMenuReference, bool IsMainMenu)
{
	HUDReference = MainMenuReference;
	bIsMainMenu = IsMainMenu;
}
