// Fill out your copyright notice in the Description page of Project Settings.


#include "ASequencePlayer.h"
#include "firepjt_firstPlayerController.h"
#include "LevelSequencePlayer.h"       
#include "MovieSceneSequencePlayer.h" 
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "PeopleBase.h"
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

	SequencePlay();
	
	cinematicUI->OpenWidgetToggle(false);
	// 텍스트 없음.
	cinematicUI->ChangeCompleteText(FString(TEXT("")));
	
	// 시퀀스 재생
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
	SequenceEnd();
	
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


// 미션 첫번째 완료되면 나올 시네마틱
void AASequencePlayer::MissionOneSequencePlay()
{
	SequencePlay();

	
	cinematicUI->ChangeCompleteText(FString(TEXT("미션1 성공!")));
	

	// 듀얼 위젯 띄우기
	cinematicUI->OpenWidgetToggle(true);
	
	if (Mission1_Fireman_Sequence && Mission1_People_Sequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		Mission1_Fireman_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission1_Fireman_Sequence, Settings, SeqActor);
		Mission1_People_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission1_People_Sequence, Settings, SeqActor);
		// 이건 시퀀스 끝나면 호출 (1번만 호출될 수 있도록 임의로 우측 시네마틱에 연결)
		Mission1_Fireman_Player->OnFinished.AddDynamic(this, &AASequencePlayer::SequenceEnd);
		// 시퀀스 재생
		Mission1_Fireman_Player->Play();
		Mission1_People_Player->Play();

		// 산소 차감 막기 위한 bool 값
		IsPlayingCinematic = true;
	}
}



// 미션 2 완료 후 나오는 시네마틱
void AASequencePlayer::MissionTwoSequencePlay()
{
	SequencePlay();
	cinematicUI->ChangeCompleteText(FString(TEXT("미션2 성공!")));
	// 듀얼 위젯 띄우기
	cinematicUI->OpenWidgetToggle(false);
	
	if (Mission2_Fireman_Sequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		Mission2_Fireman_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission2_Fireman_Sequence, Settings, SeqActor);
		// 이건 시퀀스 끝나면 호출 (1번만 호출될 수 있도록 임의로 우측 시네마틱에 연결)
		Mission2_Fireman_Player->OnFinished.AddDynamic(this, &AASequencePlayer::SequenceEnd);
		// 시퀀스 재생
		Mission2_Fireman_Player->Play();
		// 산소 차감 막기 위한 bool 값
		IsPlayingCinematic = true;
	}
}



// 미션 3 완료 후 나오는 시네마틱
void AASequencePlayer::MissionThreeSequencePlay()
{
	SequencePlay();
	cinematicUI->ChangeCompleteText(FString(TEXT("미션3 성공!")));
	
	// 단일 위젯 띄우기
	cinematicUI->OpenWidgetToggle(false);
	
	if (Mission3_Sequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		Mission3_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission3_Sequence, Settings, SeqActor);
		// 이건 시퀀스 끝나면 호출 (1번만 호출될 수 있도록 임의로 우측 시네마틱에 연결)
		Mission3_Player->OnFinished.AddDynamic(this, &AASequencePlayer::SequenceEnd);
		// 시퀀스 재생
		Mission3_Player->Play();
		// 산소 차감 막기 위한 bool 값
		IsPlayingCinematic = true;
	}
}


// 스킵 버튼
void AASequencePlayer::DoSkip()
{
	
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



// 시퀀스 시작시 호출
void AASequencePlayer::SequencePlay()
{
	// 마우스 보이기, 컨트롤 안되게 하기
	auto* pc = Cast<Afirepjt_firstPlayerController>(GetWorld()->GetFirstPlayerController());
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
	
}

// 시퀀스 끝나면 호출
void AASequencePlayer::SequenceEnd()
{
	// 산소 차감 진행
	IsPlayingCinematic = false;
	// UI 끄기
	cinematicUI->CloseWidget();

	// 마우스 숨기기, 컨트롤 되도록 수정하기
	auto* pc = Cast<Afirepjt_firstPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->bShowMouseCursor = false;
	pc->SetIgnoreLookInput(false);
	pc->SetIgnoreMoveInput(false);
	
}


// 미션 1을 위한 델리게이트 바인딩
void AASequencePlayer::BindToWidget(UPhoneWidget* InWidget)
{
	// 델리게이트 바인딩
	if (!InWidget) return;
	InWidget->OnRequestPlayCinematic.AddDynamic(this, &AASequencePlayer::MissionTwoSequencePlay);
	
}
