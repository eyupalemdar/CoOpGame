// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SWeapon.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ASWeapon::ASWeapon()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SceneComp);

	MuzzleSocketName = TEXT("MuzzleSocket");
}

/// <summary>
/// Başlangıç ve Bitiş noktası arasında bir LineTrace oluştur ve
/// bak bakalım verilen Channel'a uygun olarak herhangi bir şeye çarpıyor mu?
/// </summary>
/// <param name="Hit"></param>
/// <param name="ShotDirection"></param>
/// <returns></returns>
bool ASWeapon::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	// Kullanıcının ViewPoint bilgisini bul. (Ekrana bakış noktası)
	FVector LocationOfPlayer;
	FRotator RotationOfPlayer;
	OwnerController->GetPlayerViewPoint(LocationOfPlayer, RotationOfPlayer);
	
	// Merminin gidebileceği son noktayı hesapla
	FVector FarhestHitLocation = LocationOfPlayer + RotationOfPlayer.Vector() * MaxBulletRange;

	// Atış yönünü bul
	ShotDirection = -RotationOfPlayer.Vector();

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this); // ignore gun's itself
	CollisionQueryParams.AddIgnoredActor(GetOwner()); // ignore the user or AI who carry the gun
	CollisionQueryParams.bTraceComplex = true; // to understand Headshot etc.

	return GetWorld()->LineTraceSingleByChannel(
		Hit, LocationOfPlayer, FarhestHitLocation, ECollisionChannel::ECC_GameTraceChannel1, CollisionQueryParams);
}

/// <summary>
/// Bu silahı kullanan Pawn (Kullanıcı) ve onun Controller'ını bul
/// </summary>
/// <returns></returns>
AController* ASWeapon::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	return OwnerPawn->GetController();
}

void ASWeapon::Fire()
{
	PlayMuzzleEffectAndSound();

	if (EnableCameraShakeEffect && CameraShakeClass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetOwnerController());
		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(CameraShakeClass);
		}
	}

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccessfulHit = GunTrace(Hit, ShotDirection);

	if (bSuccessfulHit)
	{
		PlayImpactEffectAndSound(Hit.Location, ShotDirection.Rotation());

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

// Silah namlusuna efekt yerleştir
void ASWeapon::PlayMuzzleEffectAndSound()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, MeshComp, MuzzleSocketName);
}

// Merminin çarptığı noktada efekt oluştur.
void ASWeapon::PlayImpactEffectAndSound(FVector Location, FRotator Rotation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Location, Rotation);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Location, Rotation);
}
