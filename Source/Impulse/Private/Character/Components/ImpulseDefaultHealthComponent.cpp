
#include "Character/Components/ImpulseDefaultHealthComponent.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "Net/UnrealNetwork.h"

UImpulseDefaultHealthComponent::UImpulseDefaultHealthComponent()
{
	DefaultHealth = 100;

	SetIsReplicatedByDefault(true);
}

void UImpulseDefaultHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* MyOwner = GetOwner())
	{
		if (MyOwner->HasAuthority())
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UImpulseDefaultHealthComponent::HandleTakeAnyDamage);
		}
	}
	
	Health = DefaultHealth;
}

void UImpulseDefaultHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	
	if (Damage <= 0.f) { return; }
	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
	
	UE_LOG(LogTemp, Log, TEXT("(HealthComponent LOG): Health changed: %s"), *FString::SanitizeFloat(Health));
	
}

void UImpulseDefaultHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UImpulseDefaultHealthComponent, Health);
	DOREPLIFETIME(UImpulseDefaultHealthComponent, DefaultHealth);
}

