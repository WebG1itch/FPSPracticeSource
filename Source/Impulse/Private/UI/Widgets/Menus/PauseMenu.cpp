// Copyright Spitfire Labs 2022


#include "UI/Widgets/Menus/PauseMenu.h"
#include "UI/ImpulseHUD.h"
#include "Components/Button.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	B_Resume->OnReleased.AddDynamic(HUDReference, &AImpulseHUD::InGameMenu);
	B_Profile->OnReleased.AddDynamic(this, &UPauseMenu::OpenProfile);
	B_Settings->OnReleased.AddDynamic(HUDReference, &AImpulseHUD::OpenSettings);
	B_Menu->OnReleased.AddDynamic(HUDReference, &AImpulseHUD::BackToMenu);

	B_Resume->SetIsEnabled(true);
}

void UPauseMenu::OpenProfile()
{
	
}

