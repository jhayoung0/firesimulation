// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/LobbyGameMode.h"

#include "firepjt_firstPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Cubee/LobbyWidget.h"

ALobbyGameMode::ALobbyGameMode()
{
	
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ConnectedPlayers.Add(NewPlayer);

	if (ConnectedPlayers.Num() >= MaxPlayers)
	{
		OnPlayersEnough.Broadcast();
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// Array에서 제거
	APlayerController* PC = Cast<APlayerController>(Exiting);
	if (PC)
	{
		ConnectedPlayers.Remove(PC);
	}
}

void ALobbyGameMode::StartGame()
{
	// 플레이어 수 체크
	if (ConnectedPlayers.Num() < MaxPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough players!"));
		return;
	}

	GetWorld()->ServerTravel("HouseMap?listen");
}

void ALobbyGameMode::BroadcastPlayerCount()
{
	int32 CurrentPlayerCount = ConnectedPlayers.Num();

	// Iterate through all connected players and call their RPC
	for (APlayerController* PC : ConnectedPlayers)
	{
		if (PC && PC->IsValidLowLevel())
		{
			Afirepjt_firstPlayerController* CustomPC = Cast<Afirepjt_firstPlayerController>(PC);
			if (CustomPC)
			{
				CustomPC->Client_UpdatePlayerCount(CurrentPlayerCount, MaxPlayers);
			}
		}
	}
}


