// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSequencePlayer.h"       
#include "MovieSceneSequencePlayer.h" 
#include "ASequencePlayer.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "PeopleBase.h"
#include "PeopleOnePC.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Cubee/HouseGameMode.h"

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
	
}

void AASequencePlayer::PlayIntroSequence()
{

	auto* pc = Cast<APeopleOnePC>(GetWorld()->GetFirstPlayerController());
	pc->bShowMouseCursor = true;
	pc->SetIgnoreLookInput(true);
	pc->SetIgnoreMoveInput(true);
	

	
	// cinematic UI를 만들자
	if (cinematicwidget)
	{
		cinematicUI = CreateWidget<UCinematicUI>(GetWorld(), cinematicwidget);
		if (cinematicUI)
		{
			cinematicUI->AddToViewport();
		}
	}

	
	cinematicUI->OpenWidgetToggle(false);

	
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
	cinematicUI->NextWidgetStart();
	
	// 2번째 시퀀스 재생
	if (SecondSequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		SecondPlayer =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SecondSequence, Settings, SeqActor);
		if (!SecondPlayer) return;

		// 이건 시퀀스 끝나면 호출
		SecondPlayer->OnFinished.AddDynamic(this, &AASequencePlayer::OnSecondSequenceFinished);
		SecondPlayer->Play();
	}
}

void AASequencePlayer::OnSecondSequenceFinished()
{
	IsPlayingCinematic = false;
	cinematicUI->CloseWidget();

	
	auto* pc = Cast<APeopleOnePC>(GetWorld()->GetFirstPlayerController());
	pc->bShowMouseCursor = false;
	pc->SetIgnoreLookInput(false);
	pc->SetIgnoreMoveInput(false);
	

	
	// 두 번째까지 끝나면 게임 시작
	if (HasAuthority())
	{
		AHouseGameMode* GM = Cast<AHouseGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->StartGame();
		}
	}
}

void AASequencePlayer::DoSkip()
{

	auto* pc = Cast<APeopleOnePC>(GetWorld()->GetFirstPlayerController());
	pc->bShowMouseCursor = false;
	pc->SetIgnoreLookInput(false);
	pc->SetIgnoreMoveInput(false);

	
	if (FirstPlayer)
	{
		FirstPlayer->GoToEndAndStop();
	}
	if (SecondPlayer)
	{
		SecondPlayer->GoToEndAndStop();
	}

	OnSecondSequenceFinished();
}

