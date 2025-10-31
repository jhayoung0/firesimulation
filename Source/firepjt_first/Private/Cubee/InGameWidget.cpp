// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/InGameWidget.h"

#include "Components/TextBlock.h"
#include "Cubee/HouseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "MissionTable.h"

void UInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HouseGameState = Cast<AHouseGameState>(UGameplayStatics::GetGameState(this));
}

void UInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateTimer();
}

void UInGameWidget::UpdateTimer()
{
	if (!HouseGameState) return;

	if (Txt_Timer)
	{
		FString TimerText = HouseGameState->GetFormattedTimer();
		Txt_Timer->SetText(FText::FromString(TimerText));
	}
}

void UInGameWidget::SetMissionDataTable(UDataTable* NewMissionTable)
{
	MissionDataTable = NewMissionTable;
}

void UInGameWidget::SetMissionTextFromIndex(int32 Index)
{
	if (!MissionDataTable) return;
	//UE_LOG(LogTemp, Warning, TEXT("Mission Index : %d"), Index);
	
	// Row 찾기
	FName RowName = FName(*FString::FromInt(Index+1));
	FMissionTable* MissionRow = MissionDataTable->FindRow<FMissionTable>(RowName, TEXT(""));

	if (MissionRow)
	{
		if (Txt_Id)
		{
			FString MissionIdText = FString::Printf(TEXT("Mission %s"), *MissionRow->mission_id.ToString());
			Txt_Id->SetText(FText::FromString(MissionIdText));
		}

		if (Txt_Mission)
		{
			Txt_Mission->SetText(MissionRow->mission_head);
		}
		/*
		if (Txt_Description)
		{
			Txt_Description->SetText(MissionRow->mission_description);
		}
		*/
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[InGameWidget] Failed to find mission row: %s"), *RowName.ToString()); 
	}
}


