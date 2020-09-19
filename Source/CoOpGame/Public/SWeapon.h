// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;
class UPhysicalMaterial;
class UDamageType;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeapon();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float BaseDamage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float MaxBulletRange;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float HeadshotBonusPercentage;

	virtual void StartFire();

	virtual void EndFire();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UParticleSystem* FleshImpactEffect = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool EnableCameraShakeEffect = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool EnableAutoFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	float LastFireTime;

	virtual void Fire();

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	EPhysicalSurface GetSurfaceType(UPhysicalMaterial const* PhysMaterial) const;

	void PlayMuzzleEffectAndSound() const;

	void PlayImpactEffectAndSound(FVector Location, FRotator Rotation, EPhysicalSurface SurfaceType) const;

private:
	FTimerHandle TimerHandle_TimeBetweenShots;

	float TimeBetweenShots; 
};
