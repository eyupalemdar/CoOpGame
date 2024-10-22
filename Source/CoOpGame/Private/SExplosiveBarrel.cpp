// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SExplosiveBarrel.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoOpGame/Public/Components/SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
	if (CylinderMeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(CylinderMeshAsset.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> DefaultBarrelMI(TEXT("/Game/UI/M_ExplosiveBarrel_Default"));
		if (DefaultBarrelMI.Succeeded())
		{
			DefaultBarrelMaterial = DefaultBarrelMI.Object;
			MeshComp->SetMaterial(0, DefaultBarrelMaterial);
		}
		static ConstructorHelpers::FObjectFinder<UMaterial> ExplodedBarrelMI(TEXT("/Game/UI/M_ExplosiveBarrel_Exploded"));
		if (ExplodedBarrelMI.Succeeded())
		{
			ExplodedBarrelMaterial = ExplodedBarrelMI.Object;
		}
	}
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false; // prevent component from ticking and only use FireImpulse() function
	RadialForceComp->bIgnoreOwningActor = true; // ignore self

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->SetMaxHealth(70.f);

	bIsExploded = false;
	ExplosionImpulse = 400;
	ExplodeDamage = 100;
	ExplodeDamageRadius = 200;

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);
}

void ASExplosiveBarrel::ServerExplode_Implementation()
{
	Explode();
}

bool ASExplosiveBarrel::ServerExplode_Validate()
{
	return true;
}

void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* OwningHealthComp, float CurrentHealth, float DamageApplied, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealth <= 0 && !bIsExploded)
	{
		Explode();
	}
}

void ASExplosiveBarrel::Explode()
{
	if (!HasAuthority())
	{
		ServerExplode();
	}

	bIsExploded = true;

	PlayExplosiveEffect();

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this,
		ExplodeDamage, GetActorLocation(), ExplodeDamageRadius, ExplodeDamageType,
		IgnoredActors, this, GetInstigatorController(), true);

	FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
	MeshComp->AddImpulse(BoostIntensity, NAME_None, true);

	RadialForceComp->FireImpulse();
	//SetLifeSpan(10.f);
}

void ASExplosiveBarrel::PlayExplosiveEffect()
{
	if (ExplodeEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation(), GetActorRotation());
	}

	if (ExplodedBarrelMaterial)
	{
		MeshComp->SetMaterial(0, ExplodedBarrelMaterial);
	}
}

void ASExplosiveBarrel::OnRep_IsExploded()
{
	PlayExplosiveEffect();
}

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASExplosiveBarrel, bIsExploded);
}
