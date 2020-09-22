// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "CoOpGame/Public/AI/STrackerBot.h"
#include "CoOpGame/Public/Components/SHealthComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "Sound/SoundCue.h"

ASTrackerBot::ASTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200.f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->SetMaxHealth(40.f);

	bUseVelocityChange = false;
	MovementForce = 1000.f;
	RequiredDisctanceToTarget = 100.f;
	ExplodeDamage = 100.f;
	ExplodeDamageRadius = 200.f;
	SelfDamageOnOverlap = 20.f;
	SelfDamageInterval = 0.5f;
	bExploded = false;
	bSelfDestructionStarted = false;
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::OnHealthChanged);

	if (HasAuthority())
	{
		// Find initial move-to
		NextPathPoint = GetNextPathPoint();
	}
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && !bExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= RequiredDisctanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
		}
		else
		{
			// Keep moving towards next point
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;

			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		}
	}
}

//void ASTrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bSelfDestructionStarted && !bExploded)
	{
		return;
	}

	ACharacter* PlayerPawn = Cast<ACharacter>(OtherActor);
	if (PlayerPawn)
	{
		if (HasAuthority())
		{
			// Start self destruction
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDamageInterval, true, 0.f);
		}
		
		bSelfDestructionStarted = true;
	
		UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent, NAME_None);
	}
}

FVector ASTrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerCharacter);

	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1]; // First one is the ActorLocation (TrackerBot) so we need to get Next one
	}

	// That means failed to find path
	return GetActorLocation();
}

void ASTrackerBot::OnHealthChanged(USHealthComponent* OwningHealthComp, float CurrentHealth, float DamageApplied, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (MaterialInstance == nullptr)
	{
		MaterialInstance = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	else
	{
		MaterialInstance->SetScalarParameterValue(TEXT("LastTimeDamageTaken"), GetWorld()->TimeSeconds);
	}

	if (CurrentHealth <= 0)
	{
		SelfDestruct();
	}
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}

	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	UGameplayStatics::SpawnSoundAttached(ExplodeSound, RootComponent);

	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (HasAuthority())
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		UGameplayStatics::ApplyRadialDamage(this,
			ExplodeDamage, GetActorLocation(), ExplodeDamageRadius, ExplodeDamageType,
			IgnoredActors, this, GetInstigatorController(), true);

		/*Destroy();*/
		SetLifeSpan(2.f); // give clients sometime to play explosion effect
	}
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, SelfDamageOnOverlap, GetInstigatorController(), this, nullptr);
}
