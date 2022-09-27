// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/GamePlayWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UGamePlayWidget::UGamePlayWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UGamePlayWidget::FadeObject(UWidget* WidgetToChange, const float AmountToChangeBy, const float InRate)
{
	WidgetToChange->SetRenderOpacity(FMath::Clamp(WidgetToChange->GetRenderOpacity() + AmountToChangeBy, 0.f, 1.f) );


	if (WidgetToChange->GetRenderOpacity() <= 1.f && WidgetToChange->GetRenderOpacity() >= 0.f)
	{
		FTimerHandle OpacityTimerHandle;
		FTimerDelegate OpacityTimerDelegate;
		OpacityTimerDelegate.BindUFunction(this, FName("FadeObject"), WidgetToChange, AmountToChangeBy, InRate);
		GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, OpacityTimerDelegate, InRate, false);
	}
}

void UGamePlayWidget::UpdateAmmo(const int32 CurrentAmmo, const int32 MaxAmmo) const
{
	if (TB_CurrentAmmo && TB_MaxAmmo)
	{
		TB_CurrentAmmo->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
		TB_MaxAmmo->SetText(FText::FromString(FString::FromInt(MaxAmmo)));
	}
}

void UGamePlayWidget::UpdateHealth(const float CurrentHealth, const float MaxHealth) const
{
	if (PB_Health && TB_HealthAmount)
	{
		PB_Health->SetPercent(CurrentHealth / MaxHealth);
		TB_HealthAmount->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(CurrentHealth))));
	}
}

void UGamePlayWidget::OnPlayerHit(const bool HeadShot)
{
	if (HeadShot)
	{
		if (I_CritMarker)
		{
			I_CritMarker->SetRenderOpacity(1.0f);
			FadeObject(I_CritMarker, -0.02f, 0.01);
		}
	}
	else
	{
		if (I_HitMarker)
		{
			I_HitMarker->SetRenderOpacity(1.0f);
			FadeObject(I_HitMarker, -0.02f, 0.01);
		}
	}
}

void UGamePlayWidget::UpdateTimer(const int32 NewTimeInSeconds) const
{
	if (TB_Timer)
	{
		FString Seconds;
		FString Minutes;
		
		if (NewTimeInSeconds / 60 < 10)
		{
			Minutes = "0" + FString::FromInt(NewTimeInSeconds / 60);
		}
		else
		{
			Minutes = FString::FromInt(NewTimeInSeconds / 60);
		}

		if (NewTimeInSeconds % 60 < 10)
		{
			Seconds = "0" + FString::FromInt(NewTimeInSeconds % 60);
		}
		else
		{
			Seconds = FString::FromInt(NewTimeInSeconds % 60);
		}
		
		TB_Timer->SetText(FText::FromString(Minutes + ":" + Seconds));
	}
}




