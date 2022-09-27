#pragma once

#include "CoreMinimal.h"
#include "GrappleHook.h"
#include "GrappleHookCable.generated.h"

UCLASS()
class IMPULSE_API AGrappleHookCable : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* GrappleCableMesh;

	bool Attached;

public:

	AGrappleHookCable();

	void FollowGrappleHook(AGrappleHook* GrappleHook, FVector CableStart);
	

	
protected:

	virtual void BeginPlay() override;

};
