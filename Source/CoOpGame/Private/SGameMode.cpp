// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SGameMode.h"
#include "CoOpGame/Public/Components/SHealthComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

ASGameMode::ASGameMode()
{
	NumberOfBotsToSpawn = 0;
	WaveCount = 0;
	TimeBetweenWaves = 2.0f;
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_GameStates, this, &ASGameMode::CheckGameStates, 1.f, true, 0.0f);
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
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}

void ASGameMode::CheckGameStates()
{
	CheckWaveState();
	CheckAnyPlayerAlive();
}

void ASGameMode::CheckWaveState()
{
	bool bIsPreparingForNewWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NumberOfBotsToSpawn > 0 || bIsPreparingForNewWave)
	{
		return;
	}

	bool bIsPawnedBotsAlive = false;

	for (TActorIterator<APawn> It = TActorIterator<APawn>(GetWorld()); It; ++It)
	{
		APawn* CurrentPawn = *It;
		if (CurrentPawn == nullptr || CurrentPawn->IsPlayerControlled())
		{
			continue;
		}

		USHealthComponent* HealthComp = Cast<USHealthComponent>(CurrentPawn->GetComponentByClass(USHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetCurrentHealth() > 0.0f)
		{
			bIsPawnedBotsAlive = true;
			break;
		}
	}

	if (!bIsPawnedBotsAlive)
	{
		PrepareForNextWave();
		UE_LOG(LogTemp, Warning, TEXT("Wave is preparing!"));
	}
}

void ASGameMode::CheckAnyPlayerAlive()
{
	bool bIsAnyPlayerAlive = false;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController && PlayerController->GetPawn())
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(PlayerPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (HealthComp && HealthComp->GetCurrentHealth() > 0.0f)
			{
				bIsAnyPlayerAlive = true;
				break;
			}
		}
	}

	if (!bIsAnyPlayerAlive)
	{
		GameOver();
	}
}

void ASGameMode::GameOver()
{
	EndWave();

	UE_LOG(LogTemp, Warning, TEXT("Game is over"));
	// Show Game is over message etc.
}
