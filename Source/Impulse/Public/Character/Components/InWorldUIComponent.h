// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InWorldUIComponent.generated.h"


class AFloatingTextActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMPULSE_API UInWorldUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInWorldUIComponent();

private:
	UPROPERTY(EditDefaultsOnly, Category = "FloatingTextComponent")
	TSubclassOf<AFloatingTextActor> FloatingTextActorClass;

	UPROPERTY()
	TArray<AFloatingTextActor*> NameTags;

	UPROPERTY()
	TArray<AFloatingTextActor*> DamageTags;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(Client, Unreliable)
	void Client_CreateNameTag(const FText& Name, AActor* ActorToFollow);

	UFUNCTION(Client, Unreliable)
	void Client_BeginDestroyNameTag(AActor* ActorBeingFollowed);

	UFUNCTION(Client, Unreliable)
	void Client_RemoveAllNameTags();

	UFUNCTION(Client, Unreliable)
	void Client_CreateDamageNumber(int32 Damage, AActor* DamagedActor, FVector HitLocation);
	
	UPROPERTY(EditDefaultsOnly)
	float NameTagVerticalOffset = 25;
	
	bool TempBool = false;
	
private:
	UFUNCTION()
	void OnNameTagDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnDamageTagDestroyed(AActor* DestroyedActor);

};
