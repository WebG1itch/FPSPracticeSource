#include "Character/ImpulseDefaultCharacter.h"

#include "Character/Weapons/WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Character/Components/ImpulseDefaultHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Character/Components/InWorldUIComponent.h"
#include "Character/Abilities/Utility/ImpulseGrenade.h"
#include "Components/AudioComponent.h"
#include "PlayerController/ImpulsePlayerController.h"
#include "PlayerState/ImpulsePlayerState.h"
#include "Character/Weapons/MeleeBase.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Character/Components/MyCharacterMovementComponent.h"
#include "GameModes/ImpulseGameStateBase.h"
#include "Kismet/KismetMathLibrary.h"

AImpulseDefaultCharacter::AImpulseDefaultCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	/* CAN DELETE */
	// set our turn rates for input
	//TurnRateGamepad = 0.01f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(SpringArmComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(22.8f, -2.3f, 9.6f));
	Mesh1P->SetRelativeLocation(FVector(35.5f, -20.5f, -152.4f));

	Mesh1P->SetTickGroup(ETickingGroup::TG_PostUpdateWork);
	GetMesh()->SetTickGroup(ETickingGroup::TG_PostUpdateWork);

	FootStepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootStepAudioComponent"));
	FootStepAudioComponent->bAutoActivate = false;
	FootStepAudioComponent->SetupAttachment(RootComponent);
	FootStepAudioComponent->SetRelativeLocation(FVector(0.f, 0.f, -95.f));

	UseAbilityAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("UseAbilityAudioComponent"));
	UseAbilityAudioComponent->bAutoActivate = false;
	UseAbilityAudioComponent->SetupAttachment(RootComponent);
	UseAbilityAudioComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	
	// Create a collision sphere for displaying name tags when other players get close
	NearbyPlayerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("NearbyPlayers"));
	NearbyPlayerSphereComponent->SetupAttachment(RootComponent);
	NearbyPlayerSphereComponent->SetSphereRadius(2000.f);
	NearbyPlayerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AImpulseDefaultCharacter::OverlapWithNearbyPlayer);
	NearbyPlayerSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AImpulseDefaultCharacter::EndOverlapWithNearbyPlayer);
	
	WeaponIndex = 0;
	CurrentHealth = 0;

	// Component for handling text that should be displayed in world and on client only
	InWorldUIComponent = CreateDefaultSubobject<UInWorldUIComponent>(TEXT("WorldUIComponent"));

	// Component for handling health
	HealthComponent = CreateDefaultSubobject<UImpulseDefaultHealthComponent>(TEXT("HealthComponent"));

	ForwardVelocityScale = 1.f;
	BackwardVelocityScale = 1.f;
	RightVelocityScale  = 1.f;
	LeftVelocityScale = 1.f;

	SensitivityScale = 1.f;
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AImpulseDefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocallyControlled())
	{
		ImpulsePlayerController = Cast<AImpulsePlayerController>(GetController());
		if (ImpulsePlayerController)
		{
			FTimerHandle UpdateInitialHealthTimerHandle;
			GetWorldTimerManager().SetTimer(UpdateInitialHealthTimerHandle, this, &AImpulseDefaultCharacter::UpdateHUDHealth, 0.5f);
		}
	}
	
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		int32 temp1gun = UKismetMathLibrary::RandomIntegerInRange(0, 3);
		
		if (temp1gun == 0)
			CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(FirstWeaponClass, SpawnParams);
		else if (temp1gun == 1)
			CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(SecondWeaponClass, SpawnParams);
		else if (temp1gun == 2)
			CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(ThirdWeaponClass, SpawnParams);
		else if (temp1gun == 3)
			CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(FourthWeaponClass, SpawnParams);
		
		if (CurrentWeapon)
		{
			PreviousWeapon = CurrentWeapon;
			WeaponArray.Add(CurrentWeapon);
			CurrentWeapon->WeaponIsNowInHand(true);
			OnRep_AttachWeapon();
		}
		
		AWeaponBase* Weapon;
		int32 temp2gun = UKismetMathLibrary::RandomIntegerInRange(0, 3);
		
		if (temp2gun == 0 && temp1gun != 0)
			Weapon = GetWorld()->SpawnActor<AWeaponBase>(FirstWeaponClass, SpawnParams);
		else if (temp2gun == 1 && temp1gun != 1)
			Weapon = GetWorld()->SpawnActor<AWeaponBase>(SecondWeaponClass, SpawnParams);
		else if (temp2gun == 2 && temp1gun != 2)
			Weapon = GetWorld()->SpawnActor<AWeaponBase>(ThirdWeaponClass, SpawnParams);
		else if (temp2gun == 3 && temp1gun != 3)
			Weapon = GetWorld()->SpawnActor<AWeaponBase>(FourthWeaponClass, SpawnParams);
		else if (temp1gun != 1)
			Weapon = GetWorld()->SpawnActor<AWeaponBase>(SecondWeaponClass, SpawnParams);
		else
			Weapon = GetWorld()->SpawnActor<AWeaponBase>(FirstWeaponClass, SpawnParams);
		
		if (Weapon)
		{
			Weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_hand_r"));
			WeaponArray.Add(Weapon);
		}
	}
	
	HealthComponent->OnHealthChanged.AddDynamic(this, &AImpulseDefaultCharacter::OnHealthChanged);
	OnTakePointDamage.AddDynamic(this, &AImpulseDefaultCharacter::OnReceivePointDamage);
	
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		MeleeBase = GetWorld()->SpawnActor<AMeleeBase>(MeleeClass, SpawnParams);
		
		if (MeleeBase)
			MeleeBase->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	if (FootstepSoundCue->IsValidLowLevelFast())
		FootStepAudioComponent->SetSound(FootstepSoundCue);

	if (UseAbilityAudioComponent->IsValidLowLevelFast())
		UseAbilityAudioComponent->SetSound(UseAbilitySoundCue);

	if (IsLocallyControlled())
	{
		int32 temp = UKismetMathLibrary::RandomIntegerInRange(0, 3);
	
		if (temp == 0)
		{
			SetMovementAbility(MOVEMENT_Blink);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("BLINK"));
		}
		else if (temp == 1)
		{
			SetMovementAbility(MOVEMENT_Stimmy);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("STIMMY"));
		}
		else if (temp == 2)
		{
			SetMovementAbility(MOVEMENT_SlideJump);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("SLIDE JUMP"));
		}
		else if (temp == 3)
		{
			SetMovementAbility(MOVEMENT_Grapple);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("GRAPPLE"));
		}
	}
}

void AImpulseDefaultCharacter::OnRep_OnHealthChanged()
{
	UpdateHUDHealth();
}

void AImpulseDefaultCharacter::UpdateHUDHealth_Implementation()
{
	if (ImpulsePlayerController)
	{
		ImpulsePlayerController->HandleChangeHealth(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
		if (LastActorToDamage)
		{
			ImpulsePlayerController->HandleDamageIndicator(LastActorToDamage, GetActorLocation());
		}
	}
}

void AImpulseDefaultCharacter::UpdateHUDAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (ImpulsePlayerController)
	{
		ImpulsePlayerController->HandleChangeAmmo(CurrentAmmo, MaxAmmo);
	}
}

void AImpulseDefaultCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AImpulseDefaultCharacter, CurrentWeapon);
	DOREPLIFETIME(AImpulseDefaultCharacter, CurrentUtilityAbility);
	DOREPLIFETIME(AImpulseDefaultCharacter, WeaponArray);
	DOREPLIFETIME(AImpulseDefaultCharacter, MeleeBase);
	DOREPLIFETIME_CONDITION(AImpulseDefaultCharacter, CurrentHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AImpulseDefaultCharacter, LastActorToDamage, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AImpulseDefaultCharacter, CharacterPitch, COND_SkipOwner);
}

bool AImpulseDefaultCharacter::CanJumpInternal_Implementation() const
{
	return JumpIsAllowedInternal();
}

void AImpulseDefaultCharacter::ServerSwitchWeapon_Implementation(AWeaponBase* NewWeapon)
{
	CurrentWeapon = NewWeapon;
	OnRep_AttachWeapon();
}

bool AImpulseDefaultCharacter::ServerSwitchWeapon_Validate(AWeaponBase* NewWeapon)
{
	return true;
}

void AImpulseDefaultCharacter::OnRep_AttachWeapon()
{
	if (PreviousWeapon)
	{
		PreviousWeapon->WeaponIsNowInHand(false);
	}
	
	if (CurrentWeapon)
	{
		CurrentWeapon->WeaponIsNowInHand(true);
		PreviousWeapon = CurrentWeapon;
		
		if (HasAuthority() || IsLocallyControlled())
		{
			CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_hand_r"));
		}
		else
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_hand_r"));
			CurrentWeapon->GetWeaponMesh()->SetCastShadow(true);
		}
	}
}

void AImpulseDefaultCharacter::SwitchNextWeapon()
{
	if (CurrentWeapon)
	{
		if (WeaponArray.Num() > WeaponIndex + 1)
		{
			CurrentWeapon->StopFiring();
			++WeaponIndex;
			
			if (AWeaponBase* NextWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon->WeaponIsNowInHand(false);
				CurrentWeapon = NextWeapon;
				CurrentWeapon->WeaponIsNowInHand(true);
			}
		}
		else
		{
			CurrentWeapon->StopFiring();
			WeaponIndex = 0;
			
			if(AWeaponBase* NextWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon->WeaponIsNowInHand(false);
				CurrentWeapon = NextWeapon;
				CurrentWeapon->WeaponIsNowInHand(true);
			}
		}
		ServerSwitchWeapon(CurrentWeapon);
	}
}

void AImpulseDefaultCharacter::SwitchPreviousWeapon()
{
	if (CurrentWeapon)
	{
		if (WeaponIndex - 1 >= 0)
		{
			CurrentWeapon->StopFiring();
			--WeaponIndex;
			
			if (AWeaponBase* NextWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon->WeaponIsNowInHand(false);
				CurrentWeapon = NextWeapon;
				CurrentWeapon->WeaponIsNowInHand(true);
			}
		}
		else
		{
			CurrentWeapon->StopFiring();
			WeaponIndex = WeaponArray.Num() - 1;
			
			if (AWeaponBase* NextWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon->WeaponIsNowInHand(false);
				CurrentWeapon = NextWeapon;
				CurrentWeapon->WeaponIsNowInHand(true);
			}
		}
		ServerSwitchWeapon(CurrentWeapon);
	}
}

void AImpulseDefaultCharacter::SwitchToFirstWeapon()
{
	if(CurrentWeapon)
	{
		WeaponIndex = 0;
		if (CurrentWeapon != WeaponArray[WeaponIndex])
		{
			if (AWeaponBase* FirstWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon->StopFiring();
				CurrentWeapon->WeaponIsNowInHand(false);
				CurrentWeapon = FirstWeapon;
				CurrentWeapon->WeaponIsNowInHand(true);
			}
			ServerSwitchWeapon(CurrentWeapon);
		}
	}
}

void AImpulseDefaultCharacter::SwitchToSecondWeapon()
{
	if(CurrentWeapon)
	{
		WeaponIndex = 1;
		if (CurrentWeapon != WeaponArray[WeaponIndex])
		{
			if (AWeaponBase* SecondWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon->StopFiring();
				CurrentWeapon->WeaponIsNowInHand(false);
				CurrentWeapon = SecondWeapon;
				CurrentWeapon->WeaponIsNowInHand(true);
			}
			ServerSwitchWeapon(CurrentWeapon);
		}
	}
}

void AImpulseDefaultCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AImpulseDefaultCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AImpulseDefaultCharacter::StopJumping);

	// Bind Sprint events
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AImpulseDefaultCharacter::OnSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AImpulseDefaultCharacter::OnStopSprint);
	// Bind crouch events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AImpulseDefaultCharacter::OnCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AImpulseDefaultCharacter::OnStopCrouch);
	
	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AImpulseDefaultCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AImpulseDefaultCharacter::OnStopFire);

	PlayerInputComponent->BindAction("SelectFire", IE_Pressed, this, &AImpulseDefaultCharacter::ChangeWeaponFireMode);

	// Bind SwitchingWeapons
	PlayerInputComponent->BindAction("SwitchNextWeapon", IE_Pressed, this, &AImpulseDefaultCharacter::SwitchNextWeapon);
	PlayerInputComponent->BindAction("SwitchPreviousWeapon", IE_Pressed, this, &AImpulseDefaultCharacter::SwitchPreviousWeapon);

	PlayerInputComponent->BindAction("SwitchToFirstWeapon", IE_Pressed, this, &AImpulseDefaultCharacter::SwitchToFirstWeapon);
	PlayerInputComponent->BindAction("SwitchToSecondWeapon", IE_Pressed, this, &AImpulseDefaultCharacter::SwitchToSecondWeapon);
	
	// Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AImpulseDefaultCharacter::OnReload);

	// Bind melee event
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AImpulseDefaultCharacter::OnMelee);

	// Bind utility ability event
	PlayerInputComponent->BindAction("Utility", IE_Pressed, this, &AImpulseDefaultCharacter::OnBeginUtilityAbility);
	PlayerInputComponent->BindAction("Movement", IE_Pressed, this, &AImpulseDefaultCharacter::OnBeginMovementAbility);
	
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AImpulseDefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &AImpulseDefaultCharacter::MoveBackward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AImpulseDefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &AImpulseDefaultCharacter::MoveLeft);

    // Bind turn events (mouse)
	PlayerInputComponent->BindAxis("LookRight", this, &AImpulseDefaultCharacter::LookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AImpulseDefaultCharacter::LookUp);
}

void AImpulseDefaultCharacter::OnFire()
{
	if (CurrentWeapon)
		CurrentWeapon->Fire();
}

void AImpulseDefaultCharacter::OnStopFire()
{
	if (CurrentWeapon)
		CurrentWeapon->StopFiring();
}

void AImpulseDefaultCharacter::ChangeWeaponFireMode()
{
	if (CurrentWeapon)
		CurrentWeapon->ChangeFireMode();
}

void AImpulseDefaultCharacter::OnReload()
{
	if (CurrentWeapon)
		CurrentWeapon->Reload();

}

void AImpulseDefaultCharacter::OnMelee()
{
	if (MeleeBase)
		MeleeBase->OnMelee();
}

FVector AImpulseDefaultCharacter::GetCharacterLookLocation()
{
	FHitResult HitResult;

	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CameraRotation = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
	FVector End = Start + CameraRotation * 5000.f;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FCollisionResponseParams ResponseParams;

	GetWorld()->LineTraceSingleByChannel(OUT HitResult, Start, End, ECC_Visibility, CollisionParams, ResponseParams);

	FVector EndLocation = End;
	
	if (HitResult.IsValidBlockingHit())
		EndLocation = HitResult.ImpactPoint;
	
	return EndLocation;
}

void AImpulseDefaultCharacter::Jump()
{
	Super::Jump();

	GetMyMovementComponent()->SetJumping(true);

	if (IsMovementAbility(MOVEMENT_SlideJump))
		GetMyMovementComponent()->SlideJump();
}

void AImpulseDefaultCharacter::StopJumping()
{
	Super::StopJumping();

	GetMyMovementComponent()->SetJumping(false);
	
}

void AImpulseDefaultCharacter::OnSprint()
{
	GetMyMovementComponent()->SetSprinting(true);
}

void AImpulseDefaultCharacter::OnStopSprint()
{
	GetMyMovementComponent()->SetSprinting(false);
}

void AImpulseDefaultCharacter::OnCrouch()
{
	GetMyMovementComponent()->BeginCrouch();
}

void AImpulseDefaultCharacter::OnStopCrouch()
{
	GetMyMovementComponent()->EndCrouch();
}

void AImpulseDefaultCharacter::OnFootstep()
{
	FootStepAudioComponent->Play(0.f);
	
	if (GetWorld()->IsNetMode(NM_DedicatedServer))
		MultiOnFootstep();
	else
		ServerOnFootstep();
}

void AImpulseDefaultCharacter::ServerOnFootstep_Implementation()
{
	MultiOnFootstep();
}

bool AImpulseDefaultCharacter::ServerOnFootstep_Validate()
{
	return true;
}

void AImpulseDefaultCharacter::MultiOnFootstep_Implementation()
{
	if (!IsLocallyControlled() && FootStepAudioComponent)
		FootStepAudioComponent->Play(0.f);
}

bool AImpulseDefaultCharacter::MultiOnFootstep_Validate()
{
	return true;
}

void AImpulseDefaultCharacter::OnBeginUtilityAbility()
{
	if (CanUseUtilityAbility)
	{
		CanUseUtilityAbility = false;
		
		FTimerHandle ImpulseGrenadeCooldownTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ImpulseGrenadeCooldownTimerHandle, this, &AImpulseDefaultCharacter::EndUtilityCooldown, ImpulseGrenadeCooldown, false);
		
		if (UseAbilityAudioComponent->IsValidLowLevelFast())
			UseAbilityAudioComponent->Play(0.f);

		if (GetWorld()->IsNetMode(NM_DedicatedServer))
			MultiOnBeginUtilityAbilitySound();
		else
			ServerOnBeginUtilityAbilitySound();
    
		const FVector EndLocation = GetCharacterLookLocation();
		const FVector SocketLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
		const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, EndLocation);
    
		ServerOnBeginUtilityAbility(LookRotation);
	}
}

void AImpulseDefaultCharacter::EndUtilityCooldown()
{
	CanUseUtilityAbility = true;
}

void AImpulseDefaultCharacter::MultiOnBeginUtilityAbilitySound_Implementation()
{
	if (!IsLocallyControlled() && UseAbilityAudioComponent)
		UseAbilityAudioComponent->Play(0.f);
}

void AImpulseDefaultCharacter::ServerOnBeginUtilityAbilitySound_Implementation()
{
	MultiOnBeginUtilityAbilitySound();
}

void AImpulseDefaultCharacter::ServerOnBeginUtilityAbility_Implementation(FRotator LookRotation)
{
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CurrentUtilityAbility = GetWorld()->SpawnActor<AAbilityBase>(UtilityAbilityClass, SpawnParams);
	}
	
	if (CurrentUtilityAbility)
	{
		//CurrentUtilityAbility->AttachToComponent(GetFirstPersonCameraComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale); USE THIS WHEN THROW ANIMATION
		CurrentUtilityAbility->SetActorLocation(GetFirstPersonCameraComponent()->GetComponentLocation());
		CurrentUtilityAbility->SetActorRelativeRotation(LookRotation);
		CurrentUtilityAbility->BeginAbility(this, LookRotation.Vector());
	}
}

bool AImpulseDefaultCharacter::ServerOnBeginUtilityAbility_Validate(FRotator LookRotation)
{
	return true;
}

void AImpulseDefaultCharacter::OnBeginMovementAbility()
{
	if (IsMovementAbility(MOVEMENT_Blink))
	{
		GetMyMovementComponent()->DoDodge();
	}
	
	else if (IsMovementAbility(MOVEMENT_Stimmy))
	{
		GetMyMovementComponent()->StartStimmy();
	}

	else if (IsMovementAbility(MOVEMENT_Grapple))
	{
		const FVector TargetLocation = GetCharacterLookLocation();
		GetMyMovementComponent()->FireGrapple(TargetLocation, FVector(-30.f, -30.f, 50.f));
	}
}

void AImpulseDefaultCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorForwardVector(), Value * ForwardVelocityScale);
}

void AImpulseDefaultCharacter::MoveBackward(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorForwardVector(), Value * BackwardVelocityScale);
}

void AImpulseDefaultCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorRightVector(), Value * RightVelocityScale);
}

void AImpulseDefaultCharacter::MoveLeft(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorRightVector(), Value * LeftVelocityScale);
}

void AImpulseDefaultCharacter::LookRight(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value * SensitivityScale);

		if (GetLocalRole() != ROLE_SimulatedProxy)
		{
			ServerUpdateControlRotation(GetControlRotation());
		}
	}
}

void AImpulseDefaultCharacter::ServerUpdateControlRotation_Implementation(FRotator Rotation)
{
	MultiUpdateControlRotation(Rotation);
}

void AImpulseDefaultCharacter::MultiUpdateControlRotation_Implementation(FRotator Rotation)
{
	Control_Rotation = Rotation;
}

void AImpulseDefaultCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerPitchInput(Value * SensitivityScale);
		const FRotator NormalizedRot = (GetControlRotation() - GetActorRotation()).GetNormalized();
		CharacterPitch = NormalizedRot.Pitch;
		ServerLookUp(CharacterPitch);
	}
}

void AImpulseDefaultCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	if (bHasFallenOutOfWorld)
	{
		return;
	}
	bHasFallenOutOfWorld = true;
	SetActorLocation(GetActorLocation() + FVector(0, 0, 50));
	
	for (AWeaponBase* Weapon : WeaponArray)
	{
		Weapon->StopFiring();
		Weapon->Destroy();
	}
		
	bDead = true;
	
	if (IsLocallyControlled())
	{
		if (AImpulseGameStateBase* GameState = Cast<AImpulseGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
		{
			AImpulsePlayerState* SelfPlayerState = GetPlayerState<AImpulsePlayerState>();
			GameState->OnPlayerDeath(GetLocalViewingPlayerController(), SelfPlayerState, SelfPlayerState, bIsHeadShot);	
		}
	}
		
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestructTimerHandle;
	GetWorldTimerManager().SetTimer(DestructTimerHandle, this, &AImpulseDefaultCharacter::DetachFromControllerPendingDestroy, 0.1f);
	SetLifeSpan(0.1f);

	InWorldUIComponent->Client_RemoveAllNameTags();
}

void AImpulseDefaultCharacter::ServerLookUp_Implementation(const float Pitch)
{
	CharacterPitch = Pitch;
}

bool AImpulseDefaultCharacter::ServerLookUp_Validate(float Pitch)
{
	return true;
}

float AImpulseDefaultCharacter::GetCharacterPitch() const
{
	return CharacterPitch;
}

void AImpulseDefaultCharacter::OverlapWithNearbyPlayer(UPrimitiveComponent* OverlappedComponent,
AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
const FHitResult& SweepResult)
{
	if (OtherActor != this && IsLocallyControlled())
	{
 		if (AImpulseDefaultCharacter* OtherPlayer = Cast<AImpulseDefaultCharacter>(OtherActor))
 		{
 			if (AImpulsePlayerState* OtherPlayerState = Cast<AImpulsePlayerState>(OtherPlayer->GetPlayerState()))
 			{
				InWorldUIComponent->Client_CreateNameTag(FText::FromString(OtherPlayerState->GetName()), OtherActor);
 			}
 		}
	}

}

void AImpulseDefaultCharacter::EndOverlapWithNearbyPlayer(UPrimitiveComponent* OverlappedComponent,
AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this && IsLocallyControlled())
	{
		if (AImpulseDefaultCharacter* OtherPlayer = Cast<AImpulseDefaultCharacter>(OtherActor))
		{
			InWorldUIComponent->Client_BeginDestroyNameTag(OtherActor);
		}
	}
}

void AImpulseDefaultCharacter::OnReceivePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                                    FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                                    const UDamageType* DamageType, AActor* DamageCauser)
{
	if (AImpulsePlayerController* InstigatorController = Cast<AImpulsePlayerController>(InstigatedBy))
	{
		BoneName == FName("Head") ? bIsHeadShot = true : bIsHeadShot = false;
		InstigatorController->OnPlayerHit(bIsHeadShot, Damage, DamagedActor, GetMesh()->GetBoneLocation(BoneName));
	}
}

void AImpulseDefaultCharacter::OnHealthChanged(UImpulseDefaultHealthComponent* HealthComp, float Health,
                                               float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// TODO: Add instigator to list of kill assists
	LastActorToDamage = DamageCauser;
	CurrentHealth = Health;
	if (Health <= 0.f && !bDead)
	{
		for (AWeaponBase* Weapon : WeaponArray)
		{
			Weapon->StopFiring();
			Weapon->Destroy();
		}
		
		bDead = true;
		
		if (AImpulseGameStateBase* GameState = Cast<AImpulseGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
		{
			GameState->OnPlayerDeath(GetController<APlayerController>(), InstigatedBy->GetPlayerState<AImpulsePlayerState>(), GetPlayerState<AImpulsePlayerState>(), bIsHeadShot);	
		}
		
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FTimerHandle DestructTimerHandle;
		GetWorldTimerManager().SetTimer(DestructTimerHandle, this, &AImpulseDefaultCharacter::DetachFromControllerPendingDestroy, 3.5f);
		SetLifeSpan(0.1f);

		InWorldUIComponent->Client_RemoveAllNameTags();
		
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//GetMesh()->SetSimulatePhysics(true);
	}

	// TODO: Start regen timer 
}


UMyCharacterMovementComponent* AImpulseDefaultCharacter::GetMyMovementComponent() const
{
	return static_cast<UMyCharacterMovementComponent*>(GetCharacterMovement());
}

void AImpulseDefaultCharacter::SetMovementAbility(EMovementAbilities MovementAbility)
{
	if (CurrentMovementAbility != MovementAbility)
		CurrentMovementAbility = MovementAbility;
}

bool AImpulseDefaultCharacter::IsMovementAbility(EMovementAbilities MovementAbility) const
{
	return CurrentMovementAbility == MovementAbility;
}

