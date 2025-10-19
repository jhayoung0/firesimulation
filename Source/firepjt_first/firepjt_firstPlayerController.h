// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cubee/HousePlayerState.h"
#include "GameFramework/PlayerController.h"
#include "firepjt_firstPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract)
class FIREPJT_FIRST_API Afirepjt_firstPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Constructor */
	Afirepjt_firstPlayerController();

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

protected:
	// UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UPROPERTY()
	class ULobbyWidget* LobbyWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InGameWidgetClass;

	UPROPERTY()
	class UInGameWidget* InGameWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY()
	class UVictoryWidget* VictoryWidget;

	// Mission DataTable(테스트용. 추후 위치 변경 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	class UDataTable* MissionDataTable;

public:
	// Client RPC to update player count in lobby
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerCount(int32 CurrentPlayers, int32 MaxPlayers);

	// Server RPC to request current player count
	UFUNCTION(Server, Reliable)
	void Server_RequestPlayerCount();

protected:
	// Game state delegate binding
	void BindToGameStateEvents();

	UFUNCTION()
	void OnGamePhaseChanged(EGamePhase NewPhase);

protected:
	// Cinematic
	class AActor* SequenceActor;
};
