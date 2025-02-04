// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

class UPointLightComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASPowerupActor();

	void ActivatePowerup(ACharacter* ActiveForPlayer);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivated(ACharacter* ActiveForPlayer);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpired();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	USceneComponent* SceneComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Powerups")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UPointLightComponent* PointLigthComp;

	/* Time between powerup ticks */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float PowerupInterval;

	/* Total times we apply to the powerup effect */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNumberOfTicks;

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActivate)
	bool bIsPowerupActivate;

	UFUNCTION()
	void OnRep_PowerupActivate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChanged(bool bIsActive);

	UFUNCTION()
	void OnTickPowerup();

private:	
	FTimerHandle TimerHandle_PowerupTicks;

	int32 TicksProcessed;

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
