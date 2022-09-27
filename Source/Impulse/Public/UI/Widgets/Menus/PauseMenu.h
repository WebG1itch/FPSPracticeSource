// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;
class AImpulseHUD;

UCLASS()
class IMPULSE_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

	UPauseMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	
	UPROPERTY()
	AImpulseHUD* HUDReference;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Resume;

	UPROPERTY(meta = (BindWidget))
	UButton* B_Profile;

	UPROPERTY(meta = (BindWidget))
	UButton* B_Settings;
	
	UPROPERTY(meta = (BindWidget))
	UButton* B_Menu;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OpenProfile();

public:
	void SetHUDReference(AImpulseHUD* Reference) { HUDReference = Reference; }
};
