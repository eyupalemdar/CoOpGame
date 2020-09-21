// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SCharacterPlayerController.h"
#include "Blueprint/UserWidget.h"

ASCharacterPlayerController::ASCharacterPlayerController()
{

}

void ASCharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		CreateWidgets();
	}
}

void ASCharacterPlayerController::CreateWidgets()
{
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
