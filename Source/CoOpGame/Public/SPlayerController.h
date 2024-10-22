// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UUserWidget;

UCLASS()
class COOPGAME_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASPlayerController();

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
