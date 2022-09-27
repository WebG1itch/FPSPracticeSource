
#pragma once

#include "CoreMinimal.h"
#include "GridshotBall.h"
#include "Character/ImpulseDefaultCharacter.h"
#include "GameFramework/Actor.h"
#include "BeginGridshot.generated.h"

class AGridshotBall;
class UBoxComponent;

UCLASS()
class IMPULSE_API ABeginGridshot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeginGridshot();
	
	UFUNCTION()
    void HitStart(AActor* OtherActor);

	
	UPROPERTY()
	AGridshotBall* MiddleCenterBall;

	UPROPERTY()
	AGridshotBall* MiddleLeftBall;
	
	UPROPERTY()
	AGridshotBall* MiddleRightBall;
	
	UPROPERTY()
	AGridshotBall* TopCenterBall;

	UPROPERTY()
	AGridshotBall* TopLeftBall;
	
	UPROPERTY()
	AGridshotBall* TopRightBall;

	UPROPERTY()
	AGridshotBall* BottomCenterBall;

	UPROPERTY()
	AGridshotBall* BottomLeftBall;

	UPROPERTY()
	AGridshotBall* BottomRightBall;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* StartHitBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGridshotBall> BallClass;

	// distance offset
	UPROPERTY(EditDefaultsOnly)
	float DistanceOffset = -1000.f;

	// column offsets
	UPROPERTY(EditDefaultsOnly)
	float MiddleColumnOffset = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	float LeftColumnOffset = 300.f;
	
	UPROPERTY(EditDefaultsOnly)
	float RightColumnOffset = -300.f;

	// row offsets
	UPROPERTY(EditDefaultsOnly)
	float MiddleRowOffset = 400.f;
	
	UPROPERTY(EditDefaultsOnly)
	float TopRowOffset = 300.f;
	
	UPROPERTY(EditDefaultsOnly)
	float BottomRowOffset = -300.f;
	
	

	bool IsPlaying = false;
	
	float Score = 0;

	UFUNCTION()
	void HitMC(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void HitML(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HitMR(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HitTC(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void HitTL(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HitTR(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HitBC(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void HitBL(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HitBR(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);


	
	UFUNCTION()
	void HitStartComp(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION()
	void UnHideRandomBall(AActor* Actor);

	
};
