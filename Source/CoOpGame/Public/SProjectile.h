// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* SphereMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UParticleSystem* ExplodeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DamageRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DestroySelfAfterSeconds = 1.f;

	void Explode();

private:	
	FTimerHandle TimerHandle_Explode;

};
