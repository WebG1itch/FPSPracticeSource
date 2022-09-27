// Copyright Spitfire Labs 2022


#include "Networking/ImpulseGameSession.h"
#include "GameModes/ImpulseGameInstance.h"


void AImpulseGameSession::RegisterServer()
{
	UImpulseGameInstance* GameInstance = GetGameInstance<UImpulseGameInstance>();
	GameInstance->TryCreateSession();
}
