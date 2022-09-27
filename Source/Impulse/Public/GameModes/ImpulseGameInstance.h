// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

//#include "BlueprintDataDefinitions.h"
#include "ImpulseGameInstance.generated.h"


UCLASS()
class IMPULSE_API UImpulseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	bool bAllowBots;
	bool bAllowFriendlyFire;

	UImpulseGameInstance(const FObjectInitializer& ObjectInitializer);

	void TryCreateSession();
	void TryJoinSession();
	
	virtual void Init() override;
};
