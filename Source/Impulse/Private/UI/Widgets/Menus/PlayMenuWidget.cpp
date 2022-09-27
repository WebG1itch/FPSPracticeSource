// Copyright Spitfire Labs 2022


#include "UI/Widgets/Menus/PlayMenuWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "UI/MainMenuHUD.h"


void UPlayMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	B_Back->OnReleased.AddDynamic(HUDReference, &AMainMenuHUD::ReturnToPreviousWidget);
	B_Enter->OnReleased.AddDynamic(this, &UPlayMenuWidget::OnEnterButtonReleased);
}

void UPlayMenuWidget::OnEnterButtonReleased()
{
	const FString Final = "open steam." + ET_CodeInput->GetText().ToString();
	GetOwningPlayer()->ConsoleCommand(*Final);
}

void UPlayMenuWidget::SetHUDReference(AMainMenuHUD* MainMenuReference)
{
	HUDReference = MainMenuReference;
}