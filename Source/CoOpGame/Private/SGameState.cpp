// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved


#include "SGameState.h"
#include "Net/UnrealNetwork.h"

void ASGameState::SetWaveState(EWaveState NewWaveState)
{
	if (HasAuthority() && WaveState != NewWaveState)
	{
		EWaveState OldWaveState = WaveState;
		WaveState = NewWaveState; // this will call OnRep_WaveState in AllClients but not Server
		OnRep_WaveState(OldWaveState); // Implicitly call OnRep_WaveState on Server for Client-Server architecture
	}
}

void ASGameState::OnRep_WaveState(EWaveState OldWaveState)
{
	OnWaveStateChanged(WaveState, OldWaveState);
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, WaveState);
}
