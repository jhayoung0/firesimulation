// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_PlayerCount;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Firefighter;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Citizen;
	
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnFirefighterClicked();
	
	UFUNCTION()
	void OnCitizenClicked();

public:
	// Player count
	void UpdatePlayerCount(int32 Current, int32 Max);
};
