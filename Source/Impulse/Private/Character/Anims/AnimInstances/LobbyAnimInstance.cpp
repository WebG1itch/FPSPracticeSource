
#include "Character/Anims/AnimInstances/LobbyAnimInstance.h"
#include "Character/Lobby/LobbyCharacter.h"
#include "Character/Weapons/WeaponBase.h"
#include "Kismet/KismetMathLibrary.h"

ULobbyAnimInstance::ULobbyAnimInstance()
{
	
}

void ULobbyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (TryGetPawnOwner())
    		Character = Cast<ALobbyCharacter>(TryGetPawnOwner());
}

void ULobbyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		UpdateWeaponInfo(DeltaSeconds);
	}
}

void ULobbyAnimInstance::UpdateWeaponInfo(const float DeltaSeconds)
{
	if (Character)
	{
		if (Character->GetCurrentWeapon())
		{
			CurrentWeaponID = Character->GetCurrentWeapon()->GetWeaponID();
		}
		SetLeftHandIK();
	}
}

void ULobbyAnimInstance::SetLeftHandIK()
{
	if (Character)
	{
		if (Character->GetCurrentWeapon())
		{
			if (Character->GetCurrentWeapon()->GetWeaponMesh())
			{
				if (CurrentWeaponID != EWeaponID::PrototypePistol)
				{
					EnableHandIK_L = true;
					
					const FTransform TPPGunSocketTransform = Character->GetCurrentWeapon()->GetWeaponMesh()->GetSocketTransform(FName("S_TPP_LeftHand"));
					const FTransform TPMeshSocketTransform = Character->GetMesh()->GetSocketTransform(FName("hand_r"));
			
					TPP_LeftHandTransform = UKismetMathLibrary::MakeRelativeTransform(TPPGunSocketTransform, TPMeshSocketTransform);
				}
				else
				{
					EnableHandIK_L = false;
				}
			}
		}
	}
}
