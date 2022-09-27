
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/EWeaponID.h"
#include "LobbyAnimInstance.generated.h"

class ALobbyCharacter;
enum EWeaponID;

UCLASS()
class IMPULSE_API ULobbyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	ULobbyAnimInstance();

	virtual void NativeBeginPlay() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	void UpdateWeaponInfo(const float DeltaSeconds);

	void SetLeftHandIK();

	UPROPERTY(BlueprintReadOnly, Category = "References")
	ALobbyCharacter* Character;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TEnumAsByte<EWeaponID> CurrentWeaponID = NoWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "HandIK")
	FTransform TPP_LeftHandTransform;
	
	UPROPERTY(BlueprintReadOnly, Category = "Layered Blending")
	bool EnableHandIK_L = false;

};
