// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/FloatingTextActor.h"

#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widgets/InWorldUIWidget.h"

// Sets default values
AFloatingTextActor::AFloatingTextActor()
{
	ShouldFollowActor = false;
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	SetRootComponent(WidgetComponent);
	WidgetComponent->SetWidgetClass(InWorldWidget);

}

void AFloatingTextActor::SetUpNameTag(const FText& Name, AActor* ActorToFollow)
{
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	ShouldFollowActor = true;
	FollowedActor = ActorToFollow;
	NameTag = Name;
}

void AFloatingTextActor::SetUpDamageNumbers(int32 DamageToAdd, AActor* DamagedActor, FVector HitLocation)
{
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	ShouldFollowActor = false;
	FollowedActor = DamagedActor;
	UpdateDamageMarker(DamageToAdd, HitLocation);
}

void AFloatingTextActor::UpdateDamageMarker(int32 DamageToAdd, FVector HitLocation)
{
	Damage += DamageToAdd;
	if (UInWorldUIWidget* Widget = Cast<UInWorldUIWidget>(WidgetComponent->GetWidget()))
	{
		Widget->UpdateDamage(Damage);
	}

	BeginDestruction(2.f);
}

// Called when the game starts or when spawned
void AFloatingTextActor::BeginPlay()
{
	Super::BeginPlay();
	WidgetComponent->SetVisibility(true);
	WidgetComponent->SetCastShadow(false);

	if (UInWorldUIWidget* Widget = Cast<UInWorldUIWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		if (Damage > 0)
		{
			Widget->UpdateDamage(Damage);
		}
		else
		{
			Widget->SetText(NameTag);
		}
	}
}

// Called every frame
void AFloatingTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ShouldFollowActor && FollowedActor)
	{
		SetActorLocation(FollowedActor->GetActorLocation());
		
		// float NewRenderScale = FMath::Clamp(UKismetMathLibrary::Vector_Distance(GetActorLocation(), GetOwner()->GetActorLocation())*.0005, .5f, 1.f);
		// WidgetComponent->GetWidget()->SetRenderScale(FVector2D(NewRenderScale, NewRenderScale));
	}

	if (GetOwner() && WidgetComponent->GetWidgetSpace() != EWidgetSpace::Screen)
	{
		FRotator LookAtOwnerRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetOwner()->GetActorLocation());
		SetActorRotation(LookAtOwnerRotation);

	}

	if (GetWorldTimerManager().IsTimerActive(DestructionClockTimerHandle) && WidgetComponent->GetWidgetSpace() != EWidgetSpace::Screen)
	{
		if (UInWorldUIWidget* Widget = Cast<UInWorldUIWidget>(WidgetComponent->GetUserWidgetObject()))
		{
			Widget->SetRenderOpacity(FMath::Clamp(GetWorldTimerManager().GetTimerRemaining(DestructionClockTimerHandle), 0.f, 1.f));
		}
	}
}

void AFloatingTextActor::BeginDestruction(float TimeUntilDestruction)
{
	FTimerDelegate DestructionTimerDelegate;
	DestructionTimerDelegate.BindUFunction(this, FName("CallDestroy"));
	
	GetWorldTimerManager().SetTimer(DestructionClockTimerHandle, DestructionTimerDelegate, TimeUntilDestruction, false);
}

void AFloatingTextActor::ResetTimer()
{
	if (GetWorldTimerManager().IsTimerActive(DestructionClockTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DestructionClockTimerHandle);
		if (UInWorldUIWidget* Widget = Cast<UInWorldUIWidget>(WidgetComponent->GetUserWidgetObject()))
		{
			Widget->SetRenderOpacity(1);
		}
	}
}
