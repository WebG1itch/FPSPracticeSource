// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"

class AImpulseDefaultCharacter;
UCLASS()
class IMPULSE_API AAbilityBase : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "General Settings")
	UStaticMeshComponent* UtilityMesh;
	
public:	
	
	AAbilityBase();

	virtual void BeginAbility(AImpulseDefaultCharacter* Player, FVector LookRotation);

	UFUNCTION()
	virtual void OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	
	virtual void BeginPlay() override;

public:
	
	UStaticMeshComponent* GetUtilityMesh() const { return UtilityMesh; }

};
