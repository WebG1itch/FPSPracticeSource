// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InWorldUIWidget.generated.h"

class UTextBlock;

UCLASS()
class IMPULSE_API UInWorldUIWidget : public UUserWidget
{
	GENERATED_BODY()

	UInWorldUIWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_DisplayText;


public:
	void UpdateDamage(int32 Damage);
	void SetText(const FText& TextToSet) const { TB_DisplayText->SetText(TextToSet); }
};
