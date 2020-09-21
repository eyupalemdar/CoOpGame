// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCharacterPlayerController.generated.h"

class UUserWidget;

UCLASS()
class COOPGAME_API ASCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASCharacterPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UUserWidget* HUDScreen;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* HealthIndicatorScreen;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthIndicatorClass;

private:
	void CreateWidgets();
};
