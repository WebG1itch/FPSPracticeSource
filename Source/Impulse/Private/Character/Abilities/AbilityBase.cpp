// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Abilities/AbilityBase.h"

// Sets default values
AAbilityBase::AAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

}

void AAbilityBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityBase::BeginAbility(AImpulseDefaultCharacter* Player, FVector LookRotation)
{
}

void AAbilityBase::OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

