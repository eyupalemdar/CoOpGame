// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class USHealthComponent;
class UParticleSystem;
class URadialForceComponent;

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	ASExplosiveBarrel();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
	UParticleSystem* ExplodeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
	float ExplodeDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
	float ExplodeDamageRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
	TSubclassOf<UDamageType> ExplodeDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
	float ExplosionImpulse;

	UPROPERTY(ReplicatedUsing = OnRep_IsExploded) 
	bool bIsExploded;

	UFUNCTION()
	void OnRep_IsExploded();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExplode();

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwningHealthComp, float CurrentHealth, float DamageApplied, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UMaterial* DefaultBarrelMaterial;
	UMaterial* ExplodedBarrelMaterial;


	void Explode();

	void PlayExplosiveEffect();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
