// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/GameModes/DeathMatch/DeathMatchWidget.h"
#include "UI/Widgets/GameModes/DeathMatch/DMPlayerScoreWidget.h"
#include "Components/VerticalBox.h"
#include "GameModes/ImpulseGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "PlayerState/ImpulsePlayerState.h"

void UDeathMatchWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<AImpulseGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	OwningPlayerController = Cast<AImpulsePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	OwningPlayerState = Cast<AImpulsePlayerState>(GetOwningPlayerState());
	CreateLists();
	FTimerHandle UpdateWidgetTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(UpdateWidgetTimerHandle, this, &UDeathMatchWidget::UpdateLists, 0.4f, true);
}

void UDeathMatchWidget::CreateLists()
{
	Players.Empty();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (AImpulsePlayerState* NewPlayer = Cast<AImpulsePlayerState>(PlayerState))
		{
			 Players.Emplace(NewPlayer);
		}
	}
	UpdateLists();
}

void UDeathMatchWidget::UpdateLists()
{
	if (!OwningPlayerState)
	{
		OwningPlayerState = Cast<AImpulsePlayerState>(GetOwningPlayerState());
		return;
	}
	if (GameState->PlayerArray.Num() > Players.Num())
	{
		CreateLists();
		return;
	}
	OwningPlayerState->bNeedsUpdate = false;
	
	for (int i = 1; i < Players.Num(); i++)
	{
		for (int j = 0; j < Players.Num(); j++)
		{
			if (Players[i]->GetKillCount() > Players[j]->GetKillCount())
			{
				Players.Swap(i, j);
			}
			else if (Players[i]->GetKillCount() == Players[j]->GetKillCount())
			{
				if (Players[i]->GetDeathCount() < Players[j]->GetDeathCount())
				{
					Players.Swap(i, j);
				}
				else if (Players[i]->GetDeathCount() == Players[j]->GetDeathCount())
				{
					if (Players[i]->GetAssistCount() > Players[j]->GetAssistCount())
					{
						Players.Swap(i, j);
					}
				}
			}
		}
	}

	UpdateScoreboard();
}

void UDeathMatchWidget::UpdateScoreboard()
{
	if (Players.Num() > VB_PlayerScores->GetChildrenCount())
	{
		for (int i = VB_PlayerScores->GetChildrenCount(); i < Players.Num(); i++)
		{
			UDMPlayerScoreWidget* PlayerWidget = CreateWidget<UDMPlayerScoreWidget>(OwningPlayerController, PlayerScoreWidgetClass, FName("%i", VB_PlayerScores->GetChildrenCount() + 1));
			PlayerWidget->SetVisibility(ESlateVisibility::Visible);
			VB_PlayerScores->AddChild(PlayerWidget);
		}
	}
	else if (Players.Num() < VB_PlayerScores->GetChildrenCount())
	{
		for (int i = Players.Num(); i < VB_PlayerScores->GetChildrenCount(); i++)
		{
			VB_PlayerScores->RemoveChildAt(i);
		}
	}

	for (int i = 0; i < Players.Num(); i++)
	{
		FString PlayerName = Players[i]->GetName();
		int32 KillCount = Players[i]->GetKillCount();
		int32 DeathCount = Players[i]->GetDeathCount();
		int32 AssistCount = Players[i]->GetAssistCount();
		int32 Ping = Players[i]->GetPingInMilliseconds();
		bool isSelf = Players[i] == OwningPlayerState;
		
		if (UDMPlayerScoreWidget* Widget = Cast<UDMPlayerScoreWidget>(VB_PlayerScores->GetChildAt(i)))
		{
			Widget->UpdatePlayerScoreInfo(PlayerName, KillCount, DeathCount, AssistCount, Ping, isSelf);
		}
	}
}

