// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageIndicatorWidget.generated.h"

class UCanvasPanel;

UCLASS()
class IMPULSE_API UDamageIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

	UDamageIndicatorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void NativeConstruct() override;
	void RotateWidget();

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CP_Canvas;

	FVector DamageOrigin;
	FVector CurrentLocation;

	FTimerHandle TickTimerHandle;
public:
	void SetDamageLocation(FVector DamageLocation, FVector ReceiverLocation);

	void SelfDestruct();
};
