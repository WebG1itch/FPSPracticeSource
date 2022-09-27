
#include "Character/Anims/AnimNotifies/ReloadAnimNotify.h"
#include "Character/Weapons/WeaponBase.h"

void UReloadAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(AWeaponBase* Weapon = Cast<AWeaponBase>(MeshComp->GetOwner()))
		Weapon->CanGetAmmo();
}
