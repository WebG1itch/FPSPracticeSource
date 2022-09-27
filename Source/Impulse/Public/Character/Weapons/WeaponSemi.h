// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapons/WeaponBase.h"
#include "WeaponSemi.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API AWeaponSemi : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponSemi();

protected:
	virtual void BeginPlay() override;
};
