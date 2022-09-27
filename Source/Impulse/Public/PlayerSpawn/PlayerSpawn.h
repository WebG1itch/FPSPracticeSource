// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerSpawn.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API APlayerSpawn : public APlayerStart
{
	GENERATED_BODY()
public:
	
	bool bCanBeUsed = true;
	bool bNearEnemy = false;

	UPROPERTY(BlueprintReadWrite)
	bool bActive = true;

	UPROPERTY(BlueprintReadWrite)
	int Team;
};
