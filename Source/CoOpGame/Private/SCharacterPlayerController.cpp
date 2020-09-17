// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SCharacterPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASCharacterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HUDScreen = CreateWidget(this, HUDScreenClass);
	if (HUDScreen)
	{
		HUDScreen->AddToViewport();
	}
}
