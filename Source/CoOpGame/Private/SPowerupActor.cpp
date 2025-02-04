// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SPowerupActor.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ASPowerupActor::ASPowerupActor()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetRelativeLocation(FVector(0.0f));
	MeshComp->SetupAttachment(SceneComp);

	PointLigthComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLigthComp"));
	PointLigthComp->SetAttenuationRadius(200.0f);
	PointLigthComp->SetCastShadows(false);
	PointLigthComp->SetupAttachment(MeshComp);

	PowerupInterval = 10.0f;
	TotalNumberOfTicks = 1;
	TicksProcessed = 0;

	bIsPowerupActivate = false;

	SetReplicates(true);
}

void ASPowerupActor::ActivatePowerup(ACharacter* ActiveForPlayer)
{
	OnActivated(ActiveForPlayer);

	bIsPowerupActivate = true;

	OnRep_PowerupActivate();

	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTicks, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void ASPowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTicked();

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		bIsPowerupActivate = false;

		OnRep_PowerupActivate();

		OnExpired();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTicks);

		Destroy();
	}
}

void ASPowerupActor::OnRep_PowerupActivate()
{
	OnPowerupStateChanged(bIsPowerupActivate);
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsPowerupActivate);
}
