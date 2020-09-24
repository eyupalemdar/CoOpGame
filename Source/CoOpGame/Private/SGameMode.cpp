// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SGameMode.h"

ASGameMode::ASGameMode()
{
	NumberOfBotsToSpawn = 0;
	WaveCount = 0;
	TimeBetweenWaves = 30.f;
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ASGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawn--;

	if (NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	PrepareForNextWave();
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false, 0.0f);
}
