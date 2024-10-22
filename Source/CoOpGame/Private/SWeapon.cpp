// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SWeapon.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoOpGame/CoOpGame.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"

static int32 DebugShowPointDamageVariable = 0;
FAutoConsoleVariableRef CVARDebugShowPointDamage(
	TEXT("COOP.DebugShowPointDamage"),
	DebugShowPointDamageVariable,
	TEXT("Draw Point Damage At Hit Location"),
	ECVF_Cheat);

ASWeapon::ASWeapon()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SceneComp);

	MuzzleSocketName = TEXT("MuzzleSocket");

	BaseDamage = 10.f;
	MaxBulletRange = 1000.f;
	HeadshotBonusPercentage = 400.f;
	FireRate = 600.f;
	EnableAutoFire = false;

	SetReplicates(true);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	/* Bir dakika içerisinde yapılabilecek atış sayısı */
	TimeBetweenShots = 60 / FireRate;
}

void ASWeapon::StartFire()
{
	if (EnableAutoFire)
	{
		/* Gecikme süresi negatif olamaz */
		float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);

		GetWorldTimerManager().SetTimer(
			TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
	}
	else
	{
		Fire();
	}
}

void ASWeapon::EndFire()
{
	if (EnableAutoFire)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
	}
}

void ASWeapon::Fire()
{
	if (!HasAuthority())
	{
		ServerFire();
	}

	LastFireTime = GetWorld()->TimeSeconds;

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
		EPhysicalSurface SurfaceType = GetSurfaceType(Hit.PhysMaterial.Get());

		if (HasAuthority())
		{
			HitScanTrace.Location = Hit.ImpactPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}

		PlayImpactEffectAndSound(Hit.ImpactPoint, ShotDirection.Rotation(), SurfaceType);

		// Merminin çarptığı şey bir Aktör mü?
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Aktörse ona Damage uygula
			FPointDamageEvent DamageEvent(BaseDamage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();

			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= HeadshotBonusPercentage / 100;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, 
				GetOwner()->GetInstigatorController(), this, DamageType);

			if (DebugShowPointDamageVariable > 0)
			{
				DrawDebugString(GetWorld(), Hit.Location, *FString::SanitizeFloat(ActualDamage), nullptr, FColor::White, 2.f);
				DrawDebugSphere(GetWorld(), Hit.Location, 5.f, 12, FColor::Yellow, false, 2.f);
			}

			/*HitActor->TakeDamage(ActualDamage, DamageEvent, OwnerController, this);*/
		}
	}
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
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
	CollisionQueryParams.bReturnPhysicalMaterial = true; // to detect Surface Type.

	return GetWorld()->LineTraceSingleByChannel(
		Hit, LocationOfPlayer, FarhestHitLocation, WEAPON_COLLISION_CHANNEL, CollisionQueryParams);
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

EPhysicalSurface ASWeapon::GetSurfaceType(UPhysicalMaterial const* PhysMaterial) const
{
	return UPhysicalMaterial::DetermineSurfaceType(PhysMaterial);
}

// Silah namlusuna efekt yerleştir
void ASWeapon::PlayMuzzleEffectAndSound() const
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
	if (MuzzleSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, MeshComp, MuzzleSocketName);
	}
}

void ASWeapon::PlayImpactEffectAndSound(FVector ImpactPoint, FVector TraceFrom, EPhysicalSurface SurfaceType) const
{
	FVector ShotDirection = ImpactPoint - TraceFrom;
	ShotDirection.Normalize();
	PlayImpactEffectAndSound(ImpactPoint, ShotDirection.Rotation(), SurfaceType);
}

// Merminin çarptığı noktada efekt oluştur.
void ASWeapon::PlayImpactEffectAndSound(FVector ImpactPoint, FRotator Rotation, EPhysicalSurface SurfaceType) const
{
	UParticleSystem* SelectedImpactEffect = nullptr;
	switch (SurfaceType)
	{
		case SURFACE_FLESHDEFAULT:
		case SURFACE_FLESHVULNERABLE:
			SelectedImpactEffect = FleshImpactEffect;
			break;
		default:
			SelectedImpactEffect = DefaultImpactEffect;
			break;
	}

	if (SelectedImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedImpactEffect, ImpactPoint, Rotation);
	}

	if (ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, ImpactPoint, Rotation);
	}
}

void ASWeapon::OnRep_HitScanTrace()
{
	PlayMuzzleEffectAndSound();
	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	PlayImpactEffectAndSound(HitScanTrace.Location, MuzzleLocation, HitScanTrace.SurfaceType);
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASWeapon, EnableAutoFire);
	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}
