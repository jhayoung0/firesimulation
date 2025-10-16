// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/HouseGameState.h"

#include "Net/UnrealNetwork.h"

AHouseGameState::AHouseGameState()
{
	CurrentPhase = EGamePhase::WaitingToStart;
	CurrentMissionIndex = 0;
	MissionTimeRemaining = 0.f;
	MissionTimeLimit = 300.f;
}

void AHouseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHouseGameState, CurrentPhase);
	DOREPLIFETIME(AHouseGameState, CurrentMissionIndex);
	DOREPLIFETIME(AHouseGameState, MissionTimeRemaining);
	DOREPLIFETIME(AHouseGameState, MissionTimeLimit);
}

void AHouseGameState::OnRep_CurrentPhase()
{
	if (CurrentPhase != PreviousPhase)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameState] Phase changed from %d to %d"),
			(int32)PreviousPhase, (int32)CurrentPhase);

		OnPhaseChanged.Broadcast(CurrentPhase);
		PreviousPhase = CurrentPhase;
	}
}

bool AHouseGameState::AreAllPlayersMissionComplete() const
{
	if (PlayerArray.Num() == 0) return false;

	for (APlayerState* PS : PlayerArray)
	{
		AHousePlayerState* HousePS = Cast<AHousePlayerState>(PS);
		if (HousePS)
		{
			if (!HousePS->bCurrentMissionComplete)
				return false;
		}
	}

	return true;
}

FString AHouseGameState::GetFormattedTimer() const
{
	int32 Minutes = FMath::FloorToInt(MissionTimeRemaining / 60.f);
	int32 Seconds = FMath::FloorToInt(MissionTimeRemaining) % 60;

	return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
