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
	virtual void Fire();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float MaxBulletRange = 1000.f;

protected:

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
	UParticleSystem* ImpactEffect = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool EnableCameraShakeEffect = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> CameraShakeClass;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	void PlayMuzzleEffectAndSound();
	void PlayImpactEffectAndSound(FVector Location, FRotator Rotation);

private:

};
