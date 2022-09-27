// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageCounterWidget.generated.h"

class UTextBlock;

UCLASS()
class IMPULSE_API UDamageCounterWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_DamageCounter;
	
	UDamageCounterWidget(const FObjectInitializer& ObjectInitializer);

	float DamageToDisplay;
public:
	void AddDamage(float Damage);
};
