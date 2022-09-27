// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingTextActor.generated.h"


class UWidgetComponent;
class UInWorldUIWidget;

UCLASS()
class IMPULSE_API AFloatingTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingTextActor();
	void SetUpNameTag(const FText& Name, AActor* ActorToFollow);
	void SetUpDamageNumbers(int32 DamageToAdd, AActor* DamagedActor, FVector HitLocation);
	void UpdateDamageMarker(int32 DamageToAdd, FVector HitLocation);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InWorldWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool ShouldFollowActor;
	
	FText NameTag;
	
	int32 Damage = 0;
	
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WidgetComponent;
	
	FTimerHandle DestructionClockTimerHandle;

	UFUNCTION()
	void CallDestroy() { Destroy(); }

public:	
	UPROPERTY()
	AActor* FollowedActor;

	void BeginDestruction(float TimeUntilDestruction);

	void ResetTimer();

};
