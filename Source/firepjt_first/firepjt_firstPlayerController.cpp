// Copyright Epic Games, Inc. All Rights Reserved.


#include "firepjt_firstPlayerController.h"
#include "ASequencePlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "firepjt_firstCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "firepjt_first.h"
#include "Cubee/HouseGameState.h"
#include "Cubee/InGameWidget.h"
#include "Cubee/LobbyGameMode.h"
#include "Cubee/LobbyWidget.h"
#include "Cubee/VictoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Input/SVirtualJoystick.h"

Afirepjt_firstPlayerController::Afirepjt_firstPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = Afirepjt_firstCameraManager::StaticClass();
}

void Afirepjt_firstPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// phone ui를 만들자
	if (phonewidget)
	{
		phoneUI = CreateWidget<UPhoneWidget>(GetWorld(), phonewidget);
	}

	
	// 시퀀스 액터 찾아두기
	if (UWorld* W = GetWorld())
	{
		AASequencePlayer* seq = Cast<AASequencePlayer>(
			UGameplayStatics::GetActorOfClass(W, AASequencePlayer::StaticClass()));
		if (seq)
		{
			seq->BindToWidget(phoneUI);
		}
	}
	
	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(Logfirepjt_first, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}

	// 로비 위젯 및 마우스 커서 관리
	if (IsLocalController())
	{
		FString WorldName = GetWorld()->GetName();

		// Set mouse & input mode
		if (WorldName.Contains(TEXT("House")))
		{
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());

			BindToGameStateEvents();
		}
		else if (WorldName.Contains(TEXT("Lobby")))
		{
			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());

			// Create Lobby widget
			if (LobbyWidgetClass)
			{
				LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
				if (LobbyWidget)
				{
					LobbyWidget->AddToViewport();

					// Request initial player count from server
					Server_RequestPlayerCount();
				}
			}
		}
	}
}

void Afirepjt_firstPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}

}

void Afirepjt_firstPlayerController::BindToGameStateEvents()
{
	AHouseGameState* HouseGS = GetWorld()->GetGameState<AHouseGameState>();
	if (HouseGS)
	{
		HouseGS->OnPhaseChanged.AddDynamic(this, &Afirepjt_firstPlayerController::OnGamePhaseChanged);
		//UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Successfully bound to GameState events! %d"), HasAuthority())
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlayerController] GameState not ready, retrying..."));
		FTimerHandle RetryHandle;
		GetWorldTimerManager().SetTimer(RetryHandle, this, &Afirepjt_firstPlayerController::BindToGameStateEvents,
			0.1f, false);
	}
}

void Afirepjt_firstPlayerController::OnGamePhaseChanged(EGamePhase NewPhase)
{
	if (NewPhase == EGamePhase::Intro)
	{
		// 맵에 있는 Sequence player 준비
		SequenceActor = UGameplayStatics::GetActorOfClass(GetWorld(), AASequencePlayer::StaticClass());
		if (SequenceActor)
		{
			AASequencePlayer* SequencePlayer = Cast<AASequencePlayer>(SequenceActor);
			if (SequencePlayer)
			{
				SequencePlayer->PlayIntroSequence();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SequencePlayer not found!"));
		}
	}
	if (NewPhase == EGamePhase::GameStart)
	{
		if (InGameWidgetClass)
		{
			InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
			if (InGameWidget)
			{
				// DataTable 할당(추후 위치 변경 가능)
				InGameWidget->AddToViewport();
				InGameWidget->SetMissionDataTable(MissionDataTable);
			}
		}
	}
	else if (NewPhase == EGamePhase::MissionStart)
	{
		AHouseGameState* HouseGS = GetWorld()->GetGameState<AHouseGameState>();
		if (HouseGS)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current mission idx : %d"), HouseGS->CurrentMissionIndex);
			InGameWidget->SetMissionTextFromIndex(HouseGS->CurrentMissionIndex);
		}
	}
	else if (NewPhase == EGamePhase::Victory)
	{
		InGameWidget->RemoveFromParent();

		VictoryWidget = CreateWidget<UVictoryWidget>(this, VictoryWidgetClass);
		if (VictoryWidget)
		{
			VictoryWidget->AddToViewport();
		}
	}
}


void Afirepjt_firstPlayerController::Client_UpdatePlayerCount_Implementation(int32 CurrentPlayers, int32 MaxPlayers)
{
	if (LobbyWidget)
	{
		LobbyWidget->UpdatePlayerCount(CurrentPlayers, MaxPlayers);
	}
}

void Afirepjt_firstPlayerController::Server_RequestPlayerCount_Implementation()
{
	ALobbyGameMode* LobbyGM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGM)
	{
		LobbyGM->BroadcastPlayerCount();
	}
}


void Afirepjt_firstPlayerController::OpenPhoneUI()
{

	
	phoneUI->AddToViewport();
	
	bShowMouseCursor = true;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	
	AASequencePlayer* Seq = Cast<AASequencePlayer>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AASequencePlayer::StaticClass())
	);
}

void Afirepjt_firstPlayerController::ClosePhoneUI()
{
	phoneUI->RemoveFromParent();

	bShowMouseCursor = false;

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
}




