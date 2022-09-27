#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridshotBall.generated.h"

class USphereComponent;
class ABeginGridshot;

UCLASS()
class IMPULSE_API AGridshotBall : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY()
	ABeginGridshot* BeginGridShot;
	
public:	
	// Sets default values for this actor's properties
	AGridshotBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
