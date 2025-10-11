// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Logout(AController* Exiting);

	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	// 접속해 있는 컨트롤러 array
	UPROPERTY()
	TArray<APlayerController*> ConnectedPlayers;

	const int32 MaxPlayers = 2;

public:
	// UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> LobbyWidgetClass;
};
