// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/HousePlayerState.h"

#include "Net/UnrealNetwork.h"

AHousePlayerState::AHousePlayerState()
{
	PlayerRole = EPlayerRole::None;
	bCurrentMissionComplete = false;
}

void AHousePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHousePlayerState, PlayerRole);
	DOREPLIFETIME(AHousePlayerState, bCurrentMissionComplete);
}

void AHousePlayerState::SetMissionComplete()
{
	if (HasAuthority())
	{
		bCurrentMissionComplete = true;
		UE_LOG(LogTemp, Log, TEXT("[PlayerState] %s mission complete!"), *GetPlayerName()); 
	}
}

void AHousePlayerState::ResetForNextMission()
{
	if (HasAuthority())
	{
		bCurrentMissionComplete = false;
		UE_LOG(LogTemp, Log, TEXT("[PlayerState] %s reset for next mission"), *GetPlayerName());
	}
}
