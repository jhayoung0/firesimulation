// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/LobbyWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Cubee/LobbyGameMode.h"
#include "firepjt_firstPlayerController.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Start)
	{
		Btn_Start->OnClicked.AddDynamic(this, &ULobbyWidget::OnStartClicked);
	}

	if (Btn_Firefighter)
	{
		Btn_Firefighter->OnClicked.AddDynamic(this, &ULobbyWidget::OnFirefighterClicked);
	}

	if (Btn_Citizen)
	{
		Btn_Citizen->OnClicked.AddDynamic(this, &ULobbyWidget::OnCitizenClicked);
	}
}

void ULobbyWidget::OnStartClicked()
{
	//if (!bIsHost) return;

	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->StartGame();
	}
}

void ULobbyWidget::OnFirefighterClicked()
{
	// PlayerController를 통해 Server RPC 호출
	if (APlayerController* PC = GetOwningPlayer())
	{
		Afirepjt_firstPlayerController* CustomPC = Cast<Afirepjt_firstPlayerController>(PC);
		if (CustomPC)
		{
			CustomPC->Server_SetPlayerRole(EPlayerRole::Firefighter);
			UE_LOG(LogTemp, Warning, TEXT("[LobbyWidget] Firefighter role selected by %s"), *PC->GetName());
		}
	}
}

void ULobbyWidget::OnCitizenClicked()
{
	// PlayerController를 통해 Server RPC 호출
	if (APlayerController* PC = GetOwningPlayer())
	{
		Afirepjt_firstPlayerController* CustomPC = Cast<Afirepjt_firstPlayerController>(PC);
		if (CustomPC)
		{
			CustomPC->Server_SetPlayerRole(EPlayerRole::Citizen);
			UE_LOG(LogTemp, Warning, TEXT("[LobbyWidget] Citizen role selected by %s"), *PC->GetName());
		}
	}
}

void ULobbyWidget::UpdatePlayerCount(int32 Current, int32 Max)
{
	if (Txt_PlayerCount)
	{
		Txt_PlayerCount->SetText(FText::FromString(FString::Printf(TEXT("Players : %d / %d"), Current, Max)));
	}
}


