// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/InWorldUIWidget.h"

void UInWorldUIWidget::UpdateDamage(int32 Damage)
{
	// TODO: Set outline of text based on amount of damage done
	SetText(FText::FromString(FString::FromInt(Damage)));
}
