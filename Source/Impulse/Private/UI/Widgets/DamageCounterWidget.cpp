// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/DamageCounterWidget.h"
#include "Components/TextBlock.h"


UDamageCounterWidget::UDamageCounterWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DamageToDisplay = 0;
}

void UDamageCounterWidget::AddDamage(const float Damage)
{
	DamageToDisplay += Damage;
	if (TB_DamageCounter)
	{
		TB_DamageCounter->SetText(FText::FromString(FString::SanitizeFloat(DamageToDisplay)));
	}
}
