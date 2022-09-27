// Copyright Spitfire Labs 2022


#include "UI/Widgets/KillFeed/KillFeedContainerWidget.h"

#include "Components/ScrollBox.h"
#include "UI/Widgets/KillFeed/KillFeedInstanceWidget.h"

void UKillFeedContainerWidget::UpdateKillFeed(const FString& KillerName, bool bHeadShot, const FString& VictimName)
{
	UKillFeedInstanceWidget* KillFeed = CreateWidget<UKillFeedInstanceWidget>(GetOwningPlayer(), KillFeedInstanceWidgetClass, FName("%i", FMath::Rand()));
	KillFeed->SetVisibility(ESlateVisibility::Hidden);
	KillFeed->SetVariables(KillerName, bHeadShot, VictimName);
	SB_KillFeed->AddChild(KillFeed);
	HandleKillFeedOverflow();
	KillFeed->SetVisibility(ESlateVisibility::Visible);
}

void UKillFeedContainerWidget::HandleKillFeedOverflow()
{
	if (SB_KillFeed->GetChildrenCount() > 4)
	{
		SB_KillFeed->RemoveChildAt(0);
	}
}
