// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Fire();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(EditAnywhere)
	float MaxBulletRange = 1000.f;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

private:

};
