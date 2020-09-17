// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SProjectileWeapon.h"
#include "Kismet/GameplayStatics.h"

void ASProjectileWeapon::Fire()
{
	// Silah namlusuna efekt yerleştir
	UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, MeshComp, MuzzleSocketName);

	// Mermi fırlat
	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);


	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccessfulHit = GunTrace(Hit, ShotDirection);

	if (bSuccessfulHit)
	{
		// Merminin çarptığı noktada efekt oluştur.
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location, ShotDirection.Rotation());

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
