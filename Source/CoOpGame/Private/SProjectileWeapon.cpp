// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SProjectileWeapon.h"
#include "Kismet/GameplayStatics.h"

void ASProjectileWeapon::Fire()
{
	// Silah namlusuna efekt yerleştir
	PlayMuzzleEffectAndSound();

	if (EnableCameraShakeEffect && CameraShakeClass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetOwnerController());
		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(CameraShakeClass);
		}
	}

	AActor* MyOwner = GetOwner();
	// Mermiyi oluştur
	if (ProjectileClass && MyOwner)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		
		// Kullanıcının ViewPoint bilgisini bul. (Ekrana bakış noktası)
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParameters);
		Projectile->SetOwner(this);
	} 

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccessfulHit = GunTrace(Hit, ShotDirection);
	if (bSuccessfulHit)
	{
		// Merminin çarptığı noktada efekt oluştur.
		PlayImpactEffectAndSound(Hit.Location, ShotDirection.Rotation(), Hit.PhysMaterial.Get());

		// Merminin çarptığı şey bir Aktör mü?
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Aktörse ona Damage uygula
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}
