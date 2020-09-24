// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode();

	virtual void StartPlay() override;

protected:
	int32 WaveCount;

	int32 NumberOfBotsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	/* Start spawning bots */
	void StartWave();

	/* Stop spawning bots */
	void EndWave();

	/* Set timer for next start wave */
	void PrepareForNextWave();

	void CheckGameStates();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewWaveState);

private:
	FTimerHandle TimerHandle_GameStates;

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

};
