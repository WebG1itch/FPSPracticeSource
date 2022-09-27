// Copyright Spitfire Labs 2022


#include "UI/Widgets/KillFeed/KillFeedInstanceWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UKillFeedInstanceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FTimerHandle SelfDestructTimer;
	GetWorld()->GetTimerManager().SetTimer(SelfDestructTimer, this, &UKillFeedInstanceWidget::SelfDestruct, 5.0f);
}

void UKillFeedInstanceWidget::SelfDestruct()
{
	RemoveFromParent();
	Destruct();
}

void UKillFeedInstanceWidget::SetVariables(FString KillerName, bool bHeadShot, FString VictimName)
{
	TB_Killer->SetText(FText::FromString(KillerName));
	bHeadShot ? I_Image->SetColorAndOpacity(FColor::Red) : I_Image->SetColorAndOpacity(FColor::White);
	TB_DeadPlayer->SetText(FText::FromString(VictimName));
}
