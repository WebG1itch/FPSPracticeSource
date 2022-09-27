// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ImpulseHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/CustomCrosshairWidget.h"
#include "UI/Widgets/DamageCounterWidget.h"
#include "UI/Widgets/DamageIndicatorWidget.h"
#include "UI/Widgets/GamePlayWidget.h"
#include "UI/Widgets/Chat/ChatBox.h"
#include "UI/Widgets/GameModes/DeathMatch/DeathMatchWidget.h"
#include "UI/Widgets/KillFeed/KillFeedContainerWidget.h"
#include "UI/Widgets/Menus/PauseMenu.h"
#include "UI/Widgets/Menus/SettingsMenuWidget.h"

AImpulseHUD::AImpulseHUD()
{

}

void AImpulseHUD::BeginPlay()
{
	Super::BeginPlay();
	if (GamePlayWidgetClass)
	{
		GamePlayWidget = CreateWidget<UGamePlayWidget>(GetOwningPlayerController(), GamePlayWidgetClass, FName("GamePlayWidget"));
		GamePlayWidget->AddToViewport();
	}
	
	if (CustomCrosshairClass)
	{
		CustomCrosshairWidget = CreateWidget<UCustomCrosshairWidget>(GetOwningPlayerController(), CustomCrosshairClass, FName("Crosshair"));
		CustomCrosshairWidget->AddToViewport();
	}

	if (DeathMatchScoreboardClass)
	{
		DeathMatchWidget = CreateWidget<UDeathMatchWidget>(GetOwningPlayerController(), DeathMatchScoreboardClass, FName("Scoreboard"));
		DeathMatchWidget->AddToViewport();
		DeathMatchWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (KillFeedContainerWidgetClass)
	{
		KillFeedContainerWidget = CreateWidget<UKillFeedContainerWidget>(GetOwningPlayerController(), KillFeedContainerWidgetClass, FName("KillFeed"));
		KillFeedContainerWidget->AddToViewport();
	}

	if (PauseMenuClass)
	{
		PauseMenuWidget = CreateWidget<UPauseMenu>(GetOwningPlayerController(), PauseMenuClass, FName("PauseMenu"));
		PauseMenuWidget->SetHUDReference(this);
		PauseMenuWidget->AddToViewport();
		PauseMenuWidget->SetIsEnabled(true);
		PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ChatBoxClass)
	{
		ChatBoxWidget = CreateWidget<UChatBox>(GetOwningPlayerController(), ChatBoxClass, FName("ChatBox"));
		ChatBoxWidget->SetHUDReference(this);
		ChatBoxWidget->AddToViewport();
	}

	if (SettingsMenuClass)
	{
		SettingsMenuWidget = CreateWidget<USettingsMenuWidget>(GetOwningPlayerController(), SettingsMenuClass, FName("SettingsMenu"));
		SettingsMenuWidget->SetHUDReference(this, false);
		SettingsMenuWidget->AddToViewport();
		SettingsMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
}

void AImpulseHUD::DestroyDamageCounter()
{
	DamageCounterWidget->ConditionalBeginDestroy();
	DamageCounterWidget = nullptr;
}

void AImpulseHUD::OnAmmoChange(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (GamePlayWidget)
	{
		GamePlayWidget->UpdateAmmo(CurrentAmmo, MaxAmmo);
	}
}

void AImpulseHUD::UpdateKillFeed(const FString& Killer, bool bHeadShot, const FString& Victim)
{
	KillFeedContainerWidget->UpdateKillFeed(Killer, bHeadShot, Victim);
}

void AImpulseHUD::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (IsValid(GamePlayWidget))
	{
		GamePlayWidget->UpdateHealth(CurrentHealth, MaxHealth);
	}
	else
	{
		FTimerHandle UpdateHealthTimerHandle;
		FTimerDelegate TimerDelegate;

		TimerDelegate.BindUFunction(this, FName("OnHealthChanged"), CurrentHealth, MaxHealth);
		GetWorldTimerManager().SetTimer(UpdateHealthTimerHandle, TimerDelegate, 0.1f, false);
	}
}

void AImpulseHUD::BackToMenu()
{
	GetGameInstance()->GetEngine()->HandleDisconnect(GetWorld(), GetWorld()->GetNetDriver());
	
}

void AImpulseHUD::ReceiveChat(FString Name, const FText& Message, EChatTypes TypeOfChat)
{
	ChatBoxWidget->ReceiveChat(Name, Message, TypeOfChat);
}

void AImpulseHUD::InGameMenu()
{
	if (PauseMenuWidget->Visibility == ESlateVisibility::Hidden)
	{
		GetOwningPlayerController()->SetShowMouseCursor(true);
		GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI());
		PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GetOwningPlayerController()->SetShowMouseCursor(false);
		GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
		PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AImpulseHUD::Chat()
{
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	if (!OwningPlayerController->bShowMouseCursor == true)
	{
		ChatBoxWidget->ChatRequest();
		OwningPlayerController->SetShowMouseCursor(true);
		OwningPlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void AImpulseHUD::Scoreboard()
{
	DeathMatchWidget->SetVisibility(DeathMatchWidget->GetVisibility() == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AImpulseHUD::ReturnToPauseMenu()
{
	PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
	SettingsMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AImpulseHUD::OpenSettings()
{
	PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	SettingsMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void AImpulseHUD::OnPlayerHit(bool HeadShot, float Damage, AActor* DamagedActor)
{
	GamePlayWidget->OnPlayerHit(HeadShot);
	if (DamageCounterClass)
	{
		if (!DamageCounterWidget)
		{
			DamageCounterWidget = CreateWidget<UDamageCounterWidget>(GetOwningPlayerController(), DamageCounterClass, FName("DamageCounterWidget"));
			DamageCounterWidget->AddToViewport();
		}
		else
		{
			GetWorldTimerManager().ClearTimer(DamageCounterTimerHandle);
		}
		
		DamageCounterWidget->AddDamage(Damage);
		GetWorldTimerManager().SetTimer(DamageCounterTimerHandle, this, &AImpulseHUD::DestroyDamageCounter, 1.0f);
	}
}

void AImpulseHUD::UpdateTimer(int32 NewTimeInSeconds)
{
	if(GamePlayWidget && GamePlayWidgetClass)
	{
		GamePlayWidget->UpdateTimer(NewTimeInSeconds);
	}
}

void AImpulseHUD::OnTakeDamage(AActor* InstigatingActor, FVector ReceiverLocation)
{
	for (FDamageIndicatorInfo PrevInstigator : RecentDamagers)
	{
		if (InstigatingActor && PrevInstigator.ActorWhoInstigated == InstigatingActor)
		{
			GetWorldTimerManager().ClearTimer(PrevInstigator.TimerUntilRemoved);
			GetWorldTimerManager().SetTimer(PrevInstigator.TimerUntilRemoved, PrevInstigator.WidgetBeingUsed, &UDamageIndicatorWidget::SelfDestruct, 1.0f, false);
			return;
		}
	}

	UDamageIndicatorWidget* DamageIndicatorWidget = CreateWidget<UDamageIndicatorWidget>(GetOwningPlayerController(), DamageIndicatorClass, FName("DamageIndicatorWidget"));
	DamageIndicatorWidget->SetDamageLocation(InstigatingActor->GetActorLocation(), ReceiverLocation);
	DamageIndicatorWidget->AddToViewport();
	
	FDamageIndicatorInfo NewDanger;
	NewDanger.WidgetBeingUsed = DamageIndicatorWidget;
	NewDanger.ActorWhoInstigated = InstigatingActor;

	GetWorldTimerManager().SetTimer(NewDanger.TimerUntilRemoved, NewDanger.WidgetBeingUsed, &UDamageIndicatorWidget::SelfDestruct, 1.0f, false);
}

