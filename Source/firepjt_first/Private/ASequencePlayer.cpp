// Fill out your copyright notice in the Description page of Project Settings.



#include "ASequencePlayer.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "PeopleBase.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AASequencePlayer::AASequencePlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
// Called every frame
void AASequencePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called when the game starts or when spawned
void AASequencePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	
	
	 
	// cinematic UI를 만들자
	if (!cinematicUI && cinematicwidget)
	{
		cinematicUI = CreateWidget<UCinematicUI>(GetWorld(), cinematicwidget);
		if (cinematicUI)
		{
			cinematicUI->AddToViewport();
		}
	}


	
	
	if (cinematicUI)
	{
		cinematicUI->OpenWidgetToggle(false);
	}

	
	if (FirstSequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		FirstPlayer =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), FirstSequence, Settings, SeqActor);
		if (!FirstPlayer) return;

		// 이건 시퀀스 끝나면 호출
		FirstPlayer->OnFinished.AddDynamic(this, &AASequencePlayer::OnFirstSequenceFinished);
		FirstPlayer->Play();
		IsPlayingCinematic = true;
	}
	
}

void AASequencePlayer::OnFirstSequenceFinished()
{
	if (cinematicUI)
	{
		cinematicUI->NextWidgetStart();
	}
	
	// 2번째 시퀀스 재생
	if (SecondSequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		SecondPlayer =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SecondSequence, Settings, SeqActor);
		if (!SecondPlayer) return;

		// 이건 시퀀스 끝나면 호출
		SecondPlayer->OnFinished.AddDynamic(this, &AASequencePlayer::OnSequenceFinished);
		SecondPlayer->Play();
	}
}

void AASequencePlayer::OnSequenceFinished()
{
	IsPlayingCinematic = false;
	if (cinematicUI)
	{
		cinematicUI->CloseWidget();
	}
	
}

void AASequencePlayer::MissionOneSequencePlay()
{

	// cinematic UI를 만들자
	UE_LOG(LogTemp, Log, TEXT("MissionOneSequencePlay called"));


	if (!cinematicUI && cinematicwidget)
	{
		cinematicUI = CreateWidget<UCinematicUI>(GetWorld(), cinematicwidget);
		if (cinematicUI)
		{
			cinematicUI->AddToViewport();
		}
	}


	// 듀얼 화면 띄우기
	if (cinematicUI)
	{
		cinematicUI->OpenWidgetToggle(true);
	}

	
	if (Mission1_Fireman_Sequence && Mission1_People_Sequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		Mission1_Fireman_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission1_Fireman_Sequence, Settings, SeqActor);
		if (!Mission1_Fireman_Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("MissionOneSequencePlay: Failed to create Fireman sequence player"));
			return;
		}

		Mission1_People_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission1_People_Sequence, Settings, SeqActor);
		if (!Mission1_People_Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("MissionOneSequencePlay: Failed to create People sequence player"));
			return;
		}
		
		// 이건 시퀀스 끝나면 호출
		Mission1_Fireman_Player->OnFinished.AddDynamic(this, &AASequencePlayer::OnSequenceFinished);
		Mission1_Fireman_Player->Play();
		Mission1_People_Player->Play();
		
		IsPlayingCinematic = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MissionOneSequencePlay: Sequence assets are missing"));
	}
	
}

static FString NormalizeDial(FString S)
{
	S.ReplaceInline(TEXT(" "), TEXT(""));
	S.ReplaceInline(TEXT("-"), TEXT(""));
	S.ReplaceInline(TEXT("("), TEXT(""));
	S.ReplaceInline(TEXT(")"), TEXT(""));
	return S;
}


void AASequencePlayer::HandleDialCall(const FString& DialNumber)
{
	const FString Dial = NormalizeDial(DialNumber);
	if (Dial != TEXT("119")) return;

	// 듀얼 시네마틱 재생
	MissionOneSequencePlay();
	
}

void AASequencePlayer::BindToPhoneWidget(class UPhoneWidget* InWidget)
{
	if (!InWidget) return;
	InWidget->OnDialCall.AddDynamic(this, &AASequencePlayer::HandleDialCall);
	
}

