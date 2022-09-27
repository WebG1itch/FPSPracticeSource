// Copyright Spitfire Labs 2022


#include "UI/Widgets/GameModes/DeathMatch/DMPlayerScoreWidget.h"

#include "Components/TextBlock.h"

void UDMPlayerScoreWidget::UpdatePlayerScoreInfo(FString Name, int32 KillCount, int32 DeathCount, int32 AssistCount, int32 Ping, bool bIsSelf)
{
	TB_PlayerName->SetText(FText::FromString(Name));
	TB_Kills->SetText(FText::AsNumber(KillCount));
	TB_Deaths->SetText(FText::AsNumber(DeathCount));
	TB_Assists->SetText(FText::AsNumber(AssistCount));
	TB_Ping->SetText(FText::AsNumber(Ping));

	if (bIsSelf)
	{
		TB_PlayerName->SetColorAndOpacity(FColor::Cyan);
		TB_Kills->SetColorAndOpacity(FColor::Cyan);
		TB_Deaths->SetColorAndOpacity(FColor::Cyan);
		TB_Assists->SetColorAndOpacity(FColor::Cyan);
		TB_Ping->SetColorAndOpacity(FColor::Cyan);
	}
}
