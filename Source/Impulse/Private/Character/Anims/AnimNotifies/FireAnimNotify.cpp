
#include "Character/Anims/AnimNotifies/FireAnimNotify.h"
#include "Character/Weapons/WeaponBase.h"

void UFireAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(AWeaponBase* Weapon = Cast<AWeaponBase>(MeshComp->GetOwner()))
	{
		Weapon->EndedFireAnim();
	}
}
