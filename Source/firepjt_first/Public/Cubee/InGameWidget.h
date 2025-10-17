// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "InGameWidget.generated.h"

/**
 *
 */
UCLASS()
class FIREPJT_FIRST_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Timer;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Id;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Mission;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Description;

	UPROPERTY()
	class AHouseGameState* HouseGameState;

	// Mission DataTable (역할별로 다름)
	UPROPERTY(BlueprintReadWrite, Category = "Mission")
	class UDataTable* MissionDataTable;

protected:
	// Timer
	void UpdateTimer();

public:
	// Mission
	UFUNCTION(BlueprintCallable, Category = Mission)
	void SetMissionDataTable(UDataTable* NewMissionTable);
	
	void SetMissionTextFromIndex(int32 Index);

};
