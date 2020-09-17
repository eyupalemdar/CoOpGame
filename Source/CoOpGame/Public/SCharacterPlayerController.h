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
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	UUserWidget* HUDScreen;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDScreenClass;

private:

};
