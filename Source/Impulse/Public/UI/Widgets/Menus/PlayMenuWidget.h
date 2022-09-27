// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayMenuWidget.generated.h"

class UEditableText;
class UButton;
class AMainMenuHUD;

UCLASS()
class IMPULSE_API UPlayMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPlayMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UEditableText* ET_CodeInput;
	   	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Back;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Enter;

	
	UFUNCTION()
	void OnEnterButtonReleased();

	UPROPERTY()
	AMainMenuHUD* HUDReference;
	
public:
	void SetHUDReference(AMainMenuHUD* MainMenuReference);
};
