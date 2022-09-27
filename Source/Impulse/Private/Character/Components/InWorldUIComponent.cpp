// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/InWorldUIComponent.h"

#include "Character/Components/FloatingTextActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UInWorldUIComponent::UInWorldUIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInWorldUIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInWorldUIComponent::Client_RemoveAllNameTags_Implementation()
{
	for (AFloatingTextActor* Tag : NameTags)
	{
		Tag->BeginDestruction(3.0);
	}
}

void UInWorldUIComponent::Client_BeginDestroyNameTag_Implementation(AActor* ActorBeingFollowed)
{
	for (AFloatingTextActor* Tag : NameTags)
	{
		if(Tag->FollowedActor == ActorBeingFollowed)
		{
			Tag->BeginDestruction(3.0f);
		}
	}
}

void UInWorldUIComponent::Client_CreateDamageNumber_Implementation(int32 Damage, AActor* DamagedActor, FVector HitLocation)
{
	for (AFloatingTextActor* Tag : DamageTags)
	{
		if (Tag->FollowedActor == DamagedActor)
		{
			Tag->ResetTimer();
			Tag->UpdateDamageMarker(Damage, HitLocation);
			return;
		}
	}

	if (FloatingTextActorClass && DamagedActor != GetOwner())
	{
		const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, HitLocation - FVector(0.f, 0.f, 15.f));

		if (AFloatingTextActor* DamageTag = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FloatingTextActorClass, SpawnTransform, GetOwner()))
		{
			DamageTag->SetUpDamageNumbers(Damage, DamagedActor, HitLocation);
			DamageTag->OnDestroyed.AddDynamic(this, &UInWorldUIComponent::OnDamageTagDestroyed);
			UGameplayStatics::FinishSpawningActor(DamageTag, SpawnTransform);
			DamageTags.Add(DamageTag);
		}
	}
}

void UInWorldUIComponent::Client_CreateNameTag_Implementation(const FText& Name, AActor* ActorToFollow)
{
	for (AFloatingTextActor* Tag : NameTags)
	{
		if (Tag->FollowedActor == ActorToFollow)
		{
			Tag->ResetTimer();
			return;
		}
	}
	
	if (FloatingTextActorClass && ActorToFollow != GetOwner())
	{
		const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, ActorToFollow->GetActorLocation() + FVector(0.f, 0.f, NameTagVerticalOffset));
		
		if (AFloatingTextActor* NewNameTag = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FloatingTextActorClass, SpawnTransform, GetOwner()))
		{
			NewNameTag->SetUpNameTag(Name, ActorToFollow);
			NewNameTag->OnDestroyed.AddDynamic(this, &UInWorldUIComponent::OnNameTagDestroyed);
			UGameplayStatics::FinishSpawningActor(NewNameTag, SpawnTransform);
			NameTags.Add(NewNameTag);
		}
	}
}


void UInWorldUIComponent::OnNameTagDestroyed(AActor* DestroyedActor)
{
	if (AFloatingTextActor* DestroyedTag = Cast<AFloatingTextActor>(DestroyedActor))
	{
		NameTags.Remove(DestroyedTag);
	}
}

void UInWorldUIComponent::OnDamageTagDestroyed(AActor* DestroyedActor)
{
	if (AFloatingTextActor* DestroyedTag = Cast<AFloatingTextActor>(DestroyedActor))
	{
		DamageTags.Remove(DestroyedTag);
	}
}
