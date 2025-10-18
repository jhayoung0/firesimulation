// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HousePlayerState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	None			UMETA(DisplayName = "None"),
	Firefighter		UMETA(DisplayName = "Firefighter"),
	Citizen			UMETA(DisplayName = "Citizen")
};

UENUM(BlueprintType)
enum class EGamePhase :uint8
{
	WaitingToStart  UMETA(DisplayName = "Waiting"),
	GameStart       UMETA(DisplayName = "Game Start"),
	Cinematic       UMETA(DisplayName = "Cinematic"),
	MissionStart    UMETA(DisplayName = "Mission"),
	MissionComplete UMETA(DisplayName = "Mission Complete"),
	GameOver        UMETA(DisplayName = "Game Over"),
	Victory         UMETA(DisplayName = "Victory")
};

UCLASS()
class FIREPJT_FIRST_API AHousePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AHousePlayerState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// 플레이어 정보
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	EPlayerRole PlayerRole = EPlayerRole::None;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsOutOfOxygen = false;
	
public:
	// 미션 상태
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Mission")
	bool bCurrentMissionComplete = false;
	// 미션 성공(플레이어 개별 성공 시 호출)
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SetMissionComplete();
	// 미션 상태 리셋
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void ResetForNextMission();
	
};
