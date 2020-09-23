// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SPickupActor.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "CoOpGame/Public/SPowerupActor.h"
#include "GameFramework/Character.h"

ASPickupActor::ASPickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.f);
	SphereComp->SetupAttachment(RootComponent);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	DecalComp->DecalSize = FVector(64.f, 75.f, 75.f);
	DecalComp->SetupAttachment(SphereComp);

	CooldownDuration = 5.0f;
}

void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
	Respawn();
}

void ASPickupActor::Respawn()
{
	if (PowerupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s. Please update your Blueprint."), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerupInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerupClass, GetActorTransform(), SpawnParameters);

	UE_LOG(LogTemp, Warning, TEXT("Powerup Instance %s is respawned in %s."), *PowerupInstance->GetName(), *GetName());
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	/* Only Player Character can use Powerups */
	ACharacter* PlayerPawn = Cast<ACharacter>(OtherActor);

	if (PlayerPawn && PowerupInstance)
	{
		PowerupInstance->ActivatePowerup();
		PowerupInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPickupActor::Respawn, CooldownDuration, false);
	}
}
