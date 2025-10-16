// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/HouseGameMode.h"
#include "Cubee/HouseGameState.h"
#include "Cubee/HousePlayerState.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AHouseGameMode::AHouseGameMode()
{
	// GameState와 PlayerState 클래스 설정
	GameStateClass = AHouseGameState::StaticClass();
	PlayerStateClass = AHousePlayerState::StaticClass();

	// Tick 비활성화 (Timer 기반으로 동작)
	PrimaryActorTick.bCanEverTick = false;
}

void AHouseGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 초기 상태 설정
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (HouseGS)
	{
		HouseGS->CurrentPhase = EGamePhase::WaitingToStart;
		HouseGS->CurrentMissionIndex = 0;
		HouseGS->MissionTimeRemaining = 0.f;

		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Game initialized. Waiting to start..."));

		// 시네마틱 했다 치고
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AHouseGameMode::StartGame,
		1.f, false);
	}
}

void AHouseGameMode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] StartGame"));
	
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	//HouseGS->CurrentPhase = EGamePhase::Cinematic;
	//UE_LOG(LogTemp, Log, TEXT("[HouseGameMode] Game started. Playing cinematic..."));

	HouseGS->MissionTimeRemaining = HouseGS->MissionTimeLimit;

	GetWorldTimerManager().SetTimer(MissionTimerHandle, this, &AHouseGameMode::UpdateMissionTimer,
		1.f, true);

	StartMission(0);
}

void AHouseGameMode::UpdateMissionTimer()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] %s"), *HouseGS->GetFormattedTimer());
	HouseGS->MissionTimeRemaining -= 1.f;

	if (HouseGS->MissionTimeRemaining <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Time's up! Mission failed."));
		FailMission();
		return;
	}

	int32 TimeLeft = FMath::FloorToInt(HouseGS->MissionTimeRemaining);
	if (TimeLeft == 30 || TimeLeft == 10)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] %d seconds remaining!"), TimeLeft);
	}
}

void AHouseGameMode::ReportMissionComplete(APlayerController* Player)
{
	if (!Player) return;

	AHousePlayerState* HousePS = Player->GetPlayerState<AHousePlayerState>();
	if (!HousePS) return;

	HousePS->SetMissionComplete();
	UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] 개별 미션 완료"));
	
	CheckMissionProgress();
}

void AHouseGameMode::StartMission(int32 MissionIndex)
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	if (MissionIndex >= TotalMissions) return;

	// 미션 시작
	HouseGS->CurrentMissionIndex = MissionIndex;
	HouseGS->CurrentPhase = EGamePhase::Mission;

	// 서버쪽 브로드캐스트..?
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("skfhkdfgsdgfiuasg"));
		HouseGS->OnPhaseChanged.Broadcast(EGamePhase::Mission);
		HouseGS->PreviousPhase = EGamePhase::Mission;
	}
	
	// 모든 플레이어 미션 상태 리셋
	for (APlayerState* PS : HouseGS->PlayerArray)
	{
		AHousePlayerState* HousePS = Cast<AHousePlayerState>(PS);
		if (HousePS)
		{
			HousePS->ResetForNextMission();
		}
	}
}

void AHouseGameMode::CheckMissionProgress()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	if (HouseGS->AreAllPlayersMissionComplete())
	{
		AdvanceToNextMission();
	}
}

void AHouseGameMode::AdvanceToNextMission()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	int32 NextMissionIndex = HouseGS->CurrentMissionIndex + 1;

	if (NextMissionIndex >= TotalMissions)
	{
		// 승리!                                                                                                                                              
		HouseGS->CurrentPhase = EGamePhase::Victory;

		GetWorldTimerManager().ClearTimer(MissionTimerHandle);
		return;
	}

	// 다음 미션 시작
	HouseGS->CurrentPhase = EGamePhase::MissionComplete;
	
	StartMission(NextMissionIndex);
}

void AHouseGameMode::FailMission()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	HouseGS->CurrentPhase = EGamePhase::GameOver;
	
	GetWorldTimerManager().ClearTimer(MissionTimerHandle);
}

