// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "UI/Widgets/MainMenuWidget.h"
#include "UI/Widgets/Menus/PlayMenuWidget.h"
#include "UI/Widgets/Menus/SettingsMenuWidget.h"


void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetOwningPlayerController(), MainMenuClass, FName("MainMenuWidget"));
		MainMenuWidget->SetHUDReference(this);
		MainMenuWidget->AddToViewport();
	}

	if (PlayMenuClass)
	{
		PlayMenuWidget = CreateWidget<UPlayMenuWidget>(GetOwningPlayerController(), PlayMenuClass, FName("PlayMenuWidget"));
		PlayMenuWidget->SetHUDReference(this);
		PlayMenuWidget->AddToViewport();
		PlayMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (SettingsMenuClass)
	{
		SettingsMenuWidget = CreateWidget<USettingsMenuWidget>(GetOwningPlayerController(), SettingsMenuClass, FName("SettingsMenuWidget"));
		SettingsMenuWidget->SetHUDReference(this, true);
		SettingsMenuWidget->AddToViewport();
		SettingsMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	GetOwningPlayerController()->SetShowMouseCursor(true);
	GetOwningPlayerController()->SetInputMode(FInputModeUIOnly());

	RequestSettings();

	PreviousWidget = CurrentWidget = MainMenuWidget;
}

void AMainMenuHUD::SwitchWidget(const ESwitchToWidget WidgetToSwitchTo)
{
	PreviousWidget = CurrentWidget;
	CurrentWidget->SetVisibility(ESlateVisibility::Hidden);

	switch (WidgetToSwitchTo)
	{
		case MainMenu:
			CurrentWidget = MainMenuWidget;
			break;
		case PlayMenu:
			CurrentWidget = PlayMenuWidget;
			break;
		case SettingsMenu:
			CurrentWidget = SettingsMenuWidget;
			break;
			
		default: UE_LOG(LogTemp, Warning, TEXT("defaulted"));
	}

	CurrentWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMainMenuHUD::ReturnToPreviousWidget()
{
	CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	UUserWidget* TempWidget = PreviousWidget;
	PreviousWidget = CurrentWidget;
	CurrentWidget = TempWidget;
	CurrentWidget->SetVisibility(ESlateVisibility::Visible);
}

FSettings AMainMenuHUD::RequestSettings()
{
	AImpulsePlayerController* OwningPlayerController = Cast<AImpulsePlayerController>(GetOwningPlayerController());
	return OwningPlayerController->GetSettings();
}
