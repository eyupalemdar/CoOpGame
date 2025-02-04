// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UAudioComponent;
class UMaterialInstanceDynamic;
class USHealthComponent;
class USoundCue;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ASTrackerBot();

	virtual void Tick(float DeltaTime) override;

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComp;

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
	float SelfDamageInterval;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDamageOnOverlap;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* ExplodeSound;

	// power boost of the bot
	UPROPERTY(ReplicatedUsing = OnRep_PowerLevel)
	int32 PowerLevel;

	UFUNCTION()
	void OnRep_PowerLevel();

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwningHealthComp, float CurrentHealth, float DamageApplied, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();

	void SelfDestruct();

	void DamageSelf();

	// Find nearby enemies and grow in 'power level' based on amount
	void OnCheckNearbyBots();

private:	
	/* Dynamic Material to pulse on damage */
	UMaterialInstanceDynamic* MaterialInstance;

	/* Next point in navigation path */
	FVector NextPathPoint;

	bool bExploded;

	FTimerHandle TimerHandle_SelfDamage;

	bool bSelfDestructionStarted;

	const int32 MaxPowerLevel = 4;

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
