// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/HouseGameMode.h"

#include "ASequencePlayer.h"
#include "Cubee/FireGameInstance.h"
#include "Cubee/HouseGameState.h"
#include "FireMan.h"
#include "PeopleBase.h"
#include "TimerManager.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AHouseGameMode::AHouseGameMode()
{
	// GameState와 PlayerState 클래스 설정
	GameStateClass = AHouseGameState::StaticClass();
	PlayerStateClass = AHousePlayerState::StaticClass();

	// Default Pawn Classes 설정
	FirefighterPawnClass = AFireMan::StaticClass();
	CitizenPawnClass = APeopleBase::StaticClass();

	// Tick 비활성화 (Timer 기반으로 동작)
	PrimaryActorTick.bCanEverTick = false;
}

UClass* AHouseGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GameInstance에서 PlayerController의 Role 조회
	APlayerController* PC = Cast<APlayerController>(InController);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Controller is not a PlayerController, using default"));
		return Super::GetDefaultPawnClassForController(InController);
	}

	UFireGameInstance* GameInstance = GetGameInstance<UFireGameInstance>();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[HouseGameMode] FireGameInstance not found! Using default pawn."));
		return Super::GetDefaultPawnClassForController(InController);
	}

	// Role 조회
	EPlayerRole CurrentRole = GameInstance->GetPlayerRole(PC);

	// Role에 따라 Pawn Class 결정
	UClass* SelectedPawnClass = nullptr;

	switch (CurrentRole)
	{
	case EPlayerRole::Firefighter:
		SelectedPawnClass = FirefighterPawnClass;
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Spawning Firefighter pawn for %s"), *PC->GetName());
		break;

	case EPlayerRole::Citizen:
		SelectedPawnClass = CitizenPawnClass;
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Spawning Citizen pawn for %s"), *PC->GetName());
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] No role assigned for %s, using Citizen as default"), *PC->GetName());
		SelectedPawnClass = CitizenPawnClass;
		break;
	}

	// Pawn Class가 설정되지 않았으면 기본값 사용
	if (!SelectedPawnClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[HouseGameMode] Selected pawn class is null! Using default."));
		return Super::GetDefaultPawnClassForController(InController);
	}

	// PlayerState에도 Role 저장 (동기화)
	AHousePlayerState* HousePS = PC->GetPlayerState<AHousePlayerState>();
	if (HousePS && CurrentRole != EPlayerRole::None)
	{
		HousePS->PlayerRole = CurrentRole;
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Set PlayerState role to %s"),
			*UEnum::GetValueAsString(CurrentRole));
	}

	return SelectedPawnClass;
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

		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Waiting to start"));

		// 임시로 여기
		SetAllPlayersStart();
		
		// 시작 시네마틱
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			ChangeGamePhase(EGamePhase::Intro);

		}), 1.f, false);
	}
}

void AHouseGameMode::StartGame()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	ChangeGamePhase(EGamePhase::GameStart);
	
	HouseGS->MissionTimeRemaining = HouseGS->MissionTimeLimit;
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		StartMission(0);
		
	}),1.f, false);

	GetWorldTimerManager().SetTimer(MissionTimerHandle, this, &AHouseGameMode::UpdateMissionTimer,
		1.f, true);
}

void AHouseGameMode::UpdateMissionTimer()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	HouseGS->MissionTimeRemaining -= 1.f;

	if (HouseGS->MissionTimeRemaining <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] Mission failed."));
		FailMission();
		return;
	}

	/*
	int32 TimeLeft = FMath::FloorToInt(HouseGS->MissionTimeRemaining);
	if (TimeLeft == 30 || TimeLeft == 10)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] %d seconds remaining!"), TimeLeft);
	}
	*/
}

void AHouseGameMode::ReportMissionComplete(APlayerController* Player)
{
	if (!Player) return;

	AHousePlayerState* HousePS = Player->GetPlayerState<AHousePlayerState>();
	if (!HousePS) return;

	// PlayerState에서 이미 bCurrentMissionComplete를 설정했으므로 체크만 수행
	UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] %s reported mission complete"), *Player->GetName());

	CheckMissionProgress();
}

void AHouseGameMode::StartMission(int32 MissionIndex)
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	if (MissionIndex >= TotalMissions) return;

	//ChangeGamePhase(EGamePhase::MissionStart);
	
	// 미션 시작
	HouseGS->CurrentMissionIndex = MissionIndex;
	
	// 모든 플레이어 미션 상태 리셋
	for (APlayerState* PS : HouseGS->PlayerArray)
	{
		AHousePlayerState* HousePS = Cast<AHousePlayerState>(PS);
		if (HousePS)
		{
			HousePS->ResetForNextMission();
		}
	}

	ChangeGamePhase(EGamePhase::MissionStart);
}

void AHouseGameMode::CheckMissionProgress()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	UE_LOG(LogTemp, Warning, TEXT("[CheckMissionProgress] Called. Current mission: %d"), HouseGS->CurrentMissionIndex); 
	
	if (HouseGS->AreAllPlayersMissionComplete())
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckMissionProgress] All missions complete, advancing..."));
		//UE_LOG(LogTemp, Warning, TEXT("All mission complete"));
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
		ChangeGamePhase(EGamePhase::Outro);

		GetWorldTimerManager().ClearTimer(MissionTimerHandle);
		return;
	}
	
	// 다음 미션 시작
	ChangeGamePhase(EGamePhase::MissionComplete);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, NextMissionIndex]
	{
		StartMission(NextMissionIndex);
		
	}),1.f, false);
}

void AHouseGameMode::FailMission()
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;

	HouseGS->CurrentPhase = EGamePhase::GameOver;
	
	GetWorldTimerManager().ClearTimer(MissionTimerHandle);
}

void AHouseGameMode::Victory()
{
	// 승리!
	ChangeGamePhase(EGamePhase::Victory);
}

void AHouseGameMode::ChangeGamePhase(EGamePhase NewState)
{
	AHouseGameState* HouseGS = GetGameState<AHouseGameState>();
	if (!HouseGS) return;
	
	// 클라이언트쪽 상태 변경
	HouseGS->CurrentPhase = NewState;

	// 서버쪽 상태 변경 브로드캐스트
	if (HasAuthority())
	{
		HouseGS->OnPhaseChanged.Broadcast(NewState);
		HouseGS->PreviousPhase = NewState;
	}
}

void AHouseGameMode::SetAllPlayersStart()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC || !PC->GetPawn()) continue;

		AHousePlayerState* HousePS = PC->GetPlayerState<AHousePlayerState>();
		if (!HousePS) continue;

		EPlayerRole CurrentRole = HousePS->PlayerRole;
		FName PlayerStartTag;

		// Role에 따라 찾을 Tag 결정
		switch (CurrentRole)
		{
		case EPlayerRole::Firefighter:
			PlayerStartTag = FName("Firefighter");
			break;
		case EPlayerRole::Citizen:
			PlayerStartTag = FName("Citizen");
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("[HouseGameMode] No role assigned, skipping spawn"));
			continue;
		}

		// Tag로 PlayerStart 찾기
		TArray<AActor*> FoundStarts;
		UGameplayStatics::GetAllActorsOfClassWithTag(this, APlayerStart::StaticClass(), PlayerStartTag, FoundStarts);

		if (FoundStarts.Num() > 0)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(FoundStarts[0]);
			if (PlayerStart)
			{
				PC->GetPawn()->SetActorTransform(PlayerStart->GetActorTransform());
				PC->GetPawn()->SetActorHiddenInGame(false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[HouseGameMode] PlayerStart with tag '%s' not found!"), *PlayerStartTag.ToString());
		}
	}
}


