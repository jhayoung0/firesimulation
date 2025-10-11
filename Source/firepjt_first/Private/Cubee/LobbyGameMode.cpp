// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/LobbyGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Cubee/LobbyWidget.h"

ALobbyGameMode::ALobbyGameMode()
{
	
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ConnectedPlayers.Add(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("Player joined"));
	//UE_LOG(LogTemp, Display, TEXT("Player joined. Total: %d/%d"), ConnectedPlayers.Num(), MaxPlayers);
	
	// 새 플레이어에게 UI 생성
	if (LobbyWidgetClass)
	{
		ULobbyWidget* LobbyWidget = CreateWidget<ULobbyWidget>(NewPlayer, LobbyWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
			//LobbyWidget->
		}
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
		//UpdateAllPlayerUIs();
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

