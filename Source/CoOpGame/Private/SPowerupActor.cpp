// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SPowerupActor.h"

ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;
	TicksProcessed = 0;
}

void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPowerupActor::ActivatePowerup()
{
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTicks, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true, 0.f);
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
		OnExpired();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTicks);
	}
}

