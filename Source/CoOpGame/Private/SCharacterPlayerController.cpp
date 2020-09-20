// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SCharacterPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASCharacterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HUDClass)
	{
		HUDScreen = CreateWidget(this, HUDClass);
		if (HUDScreen)
		{
			HUDScreen->AddToViewport();
		}
	}

	if (HealthIndicatorClass)
	{
		HealthIndicatorScreen = CreateWidget(this, HealthIndicatorClass);
		if (HealthIndicatorScreen)
		{
			HealthIndicatorScreen->AddToViewport();
		}
	}
}
