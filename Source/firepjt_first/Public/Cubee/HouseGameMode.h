// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HousePlayerState.h"
#include "GameFramework/GameMode.h"
#include "HouseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API AHouseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AHouseGameMode();

	virtual void BeginPlay() override;

public:
	// Game (타이머 시작) 
	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartGame();
	
protected:
	// Mission
	UPROPERTY(EditDefaultsOnly, Category = "Mission")
	int32 TotalMissions = 3;

	// 타이머 핸들
	FTimerHandle MissionTimerHandle;

	// 타이머 업데이트 (1초마다 호출)
	void UpdateMissionTimer();

public:
	// 각 플레이어가 미션 성공 시 호출
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void ReportMissionComplete(APlayerController* Player);

protected:
	// 미션 시작
	void StartMission(int32 MissionIndex);
	// 모든 플레이어 미션 완료 체크
	void CheckMissionProgress();
	// 다음 미션으로
	void AdvanceToNextMission();
	// 미션 실패
	void FailMission();

protected:
	// 상태 서버&클라이언트 일괄 변경
	void ChangeGamePhase(EGamePhase NewState);
};
