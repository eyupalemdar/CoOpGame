// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved


#include "SGameState.h"
#include "Net/UnrealNetwork.h"

void ASGameState::OnRep_WaveState(EWaveState OldWaveState)
{
	OnWaveStateChanged(WaveState, OldWaveState);
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, WaveState);
}
