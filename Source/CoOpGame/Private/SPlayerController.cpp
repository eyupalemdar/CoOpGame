// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SPlayerController.h"
#include "Blueprint/UserWidget.h"

ASPlayerController::ASPlayerController()
{

}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		CreateWidgets();
	}
}

void ASPlayerController::CreateWidgets()
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
