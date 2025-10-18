// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/HousePlayerState.h"

#include "Cubee/HouseGameMode.h"
#include "Net/UnrealNetwork.h"

AHousePlayerState::AHousePlayerState()
{
	PlayerRole = EPlayerRole::None;
	bCurrentMissionComplete = false;
	bIsOutOfOxygen = false;
}

void AHousePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHousePlayerState, PlayerRole);
	DOREPLIFETIME(AHousePlayerState, bCurrentMissionComplete);
	DOREPLIFETIME(AHousePlayerState, bIsOutOfOxygen);
}

void AHousePlayerState::SetMissionComplete()
{
	if (HasAuthority())
	{
		bCurrentMissionComplete = true;

		// GameMode에 미션 완료 보고
		AHouseGameMode* GameMode = GetWorld()->GetAuthGameMode<AHouseGameMode>();
		if (GameMode)
		{
			APlayerController* PC = Cast<APlayerController>(GetOwner());
			if (PC)
			{
				GameMode->ReportMissionComplete(PC);
			}
		}
	}
}

void AHousePlayerState::ResetForNextMission()
{
	if (HasAuthority())
	{
		bCurrentMissionComplete = false;
		UE_LOG(LogTemp, Log, TEXT("[PlayerState] reset for next mission"));
	}
}
