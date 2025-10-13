// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/LobbyWidget.h"

#include "Components/Button.h"
#include "Cubee/LobbyGameMode.h"

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
	
}

void ULobbyWidget::OnCitizenClicked()
{
	
}
