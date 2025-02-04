// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SProjectile.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SWeapon.h"

static int32 DebugShotgunProjectileVariable = 0;
FAutoConsoleVariableRef CVARDebugShotgunProjectile(
	TEXT("COOP.DebugShotgunProjectile"), 
	DebugShotgunProjectileVariable,
	TEXT("Draw Radial Damage Area for Projectile Explosion"),
	ECVF_Cheat);

ASProjectile::ASProjectile()
{
	SphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComp"));
	SphereMeshComp->SetSimulatePhysics(true);
	SphereMeshComp->SetRelativeScale3D(FVector(0.25f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMeshComp->SetStaticMesh(SphereMeshAsset.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> MI(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
		if (MI.Succeeded())
		{
			SphereMeshComp->SetMaterial(0, MI.Object);
		}
	}
	RootComponent = SphereMeshComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 2000.f;
	ProjectileMovementComp->MaxSpeed = 2000.f;
}

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().ClearTimer(TimerHandle_Explode);
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASProjectile::Explode, DestroySelfAfterSeconds, false);
}

void ASProjectile::Explode()
{
	if (ExplodeEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation());
	}

	ASWeapon* Weapon = Cast<ASWeapon>(GetOwner());
	if (Weapon)
	{
		TArray<AActor*> IgnoreActors;
		UGameplayStatics::ApplyRadialDamage(this, Weapon->BaseDamage, GetActorLocation(), DamageRadius, nullptr,
			IgnoreActors, this, GetInstigatorController());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile Owner could not be found. Please set Projectile Owner."));
	}

	if (DebugShotgunProjectileVariable > 0)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 12, FColor::Yellow, false, 3.f);
	}

	Destroy();
}
