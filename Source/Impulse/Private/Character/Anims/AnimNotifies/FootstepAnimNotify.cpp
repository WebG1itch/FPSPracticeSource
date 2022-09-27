
#include "Character/Anims/AnimNotifies/FootstepAnimNotify.h"

#include "Character/ImpulseDefaultCharacter.h"

void UFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AImpulseDefaultCharacter* Player = Cast<AImpulseDefaultCharacter>(MeshComp->GetOwner()))
	{
		if (Player->IsLocallyControlled())
			Player->OnFootstep();
	}
}
