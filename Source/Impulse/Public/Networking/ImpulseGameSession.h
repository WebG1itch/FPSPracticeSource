// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "ImpulseGameSession.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API AImpulseGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	virtual void RegisterServer() override;
};