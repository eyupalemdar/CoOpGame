// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,

	WaveInProgress,

	// No longer spawning new bots, waiting for players to kill remaining bots
	WaitingToComplete,

	WaveComplete,

	GameOver
};

UCLASS()
class COOPGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SetWaveState(EWaveState NewWaveState);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState WaveState;

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldWaveState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void OnWaveStateChanged(EWaveState NewWaveState, EWaveState OldWaveState);

private:
	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
