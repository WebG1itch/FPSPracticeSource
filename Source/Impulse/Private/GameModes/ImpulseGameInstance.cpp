// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ImpulseGameInstance.h"
#include "Networking/ImpulseGameInstanceSubsystem.h"

UImpulseGameInstance::UImpulseGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UImpulseGameInstance::TryCreateSession()
{
	UImpulseGameInstanceSubsystem* GameSessionHandler = GetSubsystem<UImpulseGameInstanceSubsystem>(this);
	GameSessionHandler->CreateSession(16, 0, GetWorld()->GetName(), false, true, true, false, false, false, true, true);
}

void UImpulseGameInstance::TryJoinSession()
{
	UImpulseGameInstanceSubsystem* GameSessionHandler = GetSubsystem<UImpulseGameInstanceSubsystem>(this);
	GameSessionHandler->FindSessions(10000, false);
}

void UImpulseGameInstance::Init()
{
	Super::Init();

	UImpulseGameInstanceSubsystem* GameSessionHandler = GetSubsystem<UImpulseGameInstanceSubsystem>(this);
	GameSessionHandler->Login();
}
