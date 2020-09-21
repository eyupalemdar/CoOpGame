// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;
class USHealthComponent;
class UMaterialInstanceDynamic;
class USphereComponent;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ASTrackerBot();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDisctanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplodeDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplodeDamageRadius;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	TSubclassOf<UDamageType> ExplodeDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDamageOnOverlap;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwningHealthComp, float CurrentHealth, float DamageApplied, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();

	void SelfDestruct();

	void DamageSelf();

private:	
	/* Dynamic Material to pulse on damage */
	UMaterialInstanceDynamic* MaterialInstance;

	/* Next point in navigation path */
	FVector NextPathPoint;

	bool bExploded;

	FTimerHandle TimerHandle_SelfDamage;

	bool bSelfDestructionStarted;
};
