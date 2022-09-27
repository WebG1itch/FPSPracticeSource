// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "UI/MainMenuHUD.h"
// #include "GameModes/ImpulseGameInstance.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	B_ExitGame->OnReleased.AddDynamic(this, &UMainMenuWidget::OnExitGameButtonReleased);
	B_Play->OnReleased.AddDynamic(this, &UMainMenuWidget::OnPlayButtonReleased);
	
	B_Settings->OnReleased.AddDynamic(this, &UMainMenuWidget::OnSettingsButtonReleased);

	TB_Name->SetText(FText::FromString(HUDReference->GetOwningPlayerController()->GetPlayerState<APlayerState>()->GetPlayerName()));
}

void UMainMenuWidget::OnExitGameButtonReleased()
{
	FGenericPlatformMisc::RequestExit(false);
	// TODO: Open an "Are you sure?" prompt
}

void UMainMenuWidget::OnPlayButtonReleased()
{
	HUDReference->SwitchWidget(AMainMenuHUD::PlayMenu);
}

void UMainMenuWidget::OnSettingsButtonReleased()
{
	HUDReference->SwitchWidget(AMainMenuHUD::SettingsMenu);
}

void UMainMenuWidget::SetHUDReference(AMainMenuHUD* MainMenuReference)
{
	HUDReference = MainMenuReference;
}