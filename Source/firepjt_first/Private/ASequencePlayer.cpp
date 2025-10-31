// Fill out your copyright notice in the Description page of Project Settings.


#include "ASequencePlayer.h"

#include "FireMan.h"
#include "firepjt_firstPlayerController.h"
#include "LevelSequencePlayer.h"       
#include "MovieSceneSequencePlayer.h" 
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "PeopleBase.h"
#include "Components/AudioComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Cubee/HouseGameMode.h"

// Sets default values
AASequencePlayer::AASequencePlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
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

	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		ACinematicSC::StaticClass(), Actors);
	
	for (AActor* A : Actors)
	{
		TArray<UActorComponent*> Components;
		A->GetComponents(Components);

		for (UActorComponent* Comp : Components)
		{
			if (!Comp) continue;

			if (!leftSC && Comp->ComponentHasTag("LEFT_SC"))
				leftSC = Cast<ACinematicSC>(A);

			else if (!rightSC && Comp->ComponentHasTag("RIGHT_SC"))
				rightSC = Cast<ACinematicSC>(A);

			if (leftSC && rightSC)
				break;
		}
	}

	
}

void AASequencePlayer::PlayIntroSequence()
{
	SequencePlay();
	
	cinematicUI->OpenWidgetToggle(2);
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
	cinematicUI->OpenWidgetToggle(3);
	
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
	// 스킵버튼 숨기기
	cinematicUI->HideSkipbtn();
	// 미션 성공 UI 띄우기
	cinematicUI->ChangeCompleteText(FString(TEXT("미션1 성공!")));
	
	// 듀얼 위젯 띄우기
	cinematicUI->OpenWidgetToggle(1);
	
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

// 미션1
void AASequencePlayer::ServerRPC_MissionOneSequencePlay_Implementation()
{
	MultiCastRPC_MissionOneSequencePlay();
}

void AASequencePlayer::MultiCastRPC_MissionOneSequencePlay_Implementation()
{
	MissionOneSequencePlay();
}


// 미션 2 완료 후 나오는 시네마틱
void AASequencePlayer::MissionTwoSequencePlay()
{
	SequencePlay();

	// 스킵버튼 숨기기
	cinematicUI->HideSkipbtn();
	cinematicUI->ChangeCompleteText(FString(TEXT("미션2 성공!")));
	cinematicUI->OpenWidgetToggle(2);
	
	if (Mission2_Fireman_Sequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		Mission2_Fireman_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission2_Fireman_Sequence, Settings, SeqActor);
		// 이건 시퀀스 끝나면 호출
		Mission2_Fireman_Player->OnFinished.AddDynamic(this, &AASequencePlayer::SequenceEnd);
		// 시퀀스 재생
		Mission2_Fireman_Player->Play();
		// 산소 차감 막기 위한 bool 값
		IsPlayingCinematic = true;
	}
}

void AASequencePlayer::ServerRPC_MissionTwoSequencePlay_Implementation()
{
	MultiCastRPC_MissionTwoSequencePlay();
}

void AASequencePlayer::MultiCastRPC_MissionTwoSequencePlay_Implementation()
{
	MissionTwoSequencePlay();
}


// 미션 3 완료 후 나오는 시네마틱
void AASequencePlayer::MissionThreeSequencePlay()
{
	UE_LOG(LogTemp, Warning, TEXT("play multicast third sequence"));
	
	SequencePlay();
	cinematicUI->ChangeCompleteText(FString(TEXT("미션3 성공!")));

	// 스킵버튼 숨기기
	cinematicUI->HideSkipbtn();
	
	// 단일 위젯 띄우기
	cinematicUI->OpenWidgetToggle(3);
	
	if (Mission3_Sequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mission3_Sequence 존재"));
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SeqActor = nullptr;
		Mission3_Player =
			ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Mission3_Sequence, Settings, SeqActor);
		// 이건 시퀀스 끝나면 호출 (1번만 호출될 수 있도록 임의로 우측 시네마틱에 연결)
		Mission3_Player->OnFinished.AddDynamic(this, &AASequencePlayer::LastSequenceFinished);
		// 시퀀스 재생
		Mission3_Player->Play();
		// 산소 차감 막기 위한 bool 값
		IsPlayingCinematic = true;
	}
}

void AASequencePlayer::LastSequenceFinished()
{
	
	SequenceEnd();

	// 배경음악 끄기
	if (BGM_Sound) 
	{
		// 마스크 sound 끄기
		if (BGMComp && BGMComp->IsPlaying())
		{
			BGMComp->Stop();
		}
	}
	
	// 마지막 끝나면 Victory로 스테이트 변경
	if (HasAuthority())
	{
		AHouseGameMode* GM = Cast<AHouseGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->Victory();
		}
	}
}



void AASequencePlayer::ServerRPC_MissionThreeSequencePlay_Implementation()
{
	MultiCastRPC_MissionThreeSequencePlay();
}

void AASequencePlayer::MultiCastRPC_MissionThreeSequencePlay_Implementation()
{
	MissionThreeSequencePlay();
}

// 시퀀스 시작시 호출
void AASequencePlayer::SequencePlay()
{
	// 배경음악 끄기
	if (BGM_Sound) 
	{
		// 마스크 sound 끄기
		if (BGMComp && BGMComp->IsPlaying())
		{
			BGMComp->Stop();
		}
	}

	// 시네마틱 켜서 액터들 숨기기
	SetCinematicActive(true);
	
	// 씬 캡쳐 on
	SetCaptureActive(leftSC,  true);
	SetCaptureActive(rightSC, true);

	// 첫 프레임 끊김 방지용 한 번 캡처
	if (leftSC)  leftSC->GetCaptureComponent2D()->CaptureScene();
	if (rightSC) rightSC->GetCaptureComponent2D()->CaptureScene();


	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->IsLocalController())
		{
			FirePC = Cast<Afirepjt_firstPlayerController>(PC);
			if (FirePC)
			{
				FirePC->bShowMouseCursor = true;
				FirePC->SetIgnoreLookInput(true);
				FirePC->SetIgnoreMoveInput(true);

			
				
				if (cinematicwidget)
				{
					cinematicUI = CreateWidget<UCinematicUI>(FirePC, cinematicwidget);
					if (cinematicUI)
					{
						cinematicUI->AddToViewport();
					}
				}
			}
		}
	}
}

// 시퀀스 끝나면 호출
void AASequencePlayer::SequenceEnd()
{
	// 배경음악 다시 시작
	if (BGM_Sound) 
	{
		// AudioComponent를 생성하고 루프 설정
		BGMComp = UGameplayStatics::SpawnSound2D(GetWorld(), BGM_Sound, 1.0f, 1.0f, 0.0f, nullptr, true);
	}

	
	// 시네마틱 꺼서 액터들 표시
	SetCinematicActive(false);
	
	// 씬 캡쳐 off
	SetCaptureActive(leftSC,  false);
	SetCaptureActive(rightSC, false);
		
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
	InWidget->OnRequestPlayCinematic.AddDynamic(this, &AASequencePlayer::ServerRPC_MissionOneSequencePlay);

}


void AASequencePlayer::SetCaptureActive(ACinematicSC* CaptureActor,
	bool bEnable)
{
	
	if (USceneCaptureComponent2D* SC = CaptureActor->GetCaptureComponent2D())
	{
		SC->bCaptureEveryFrame = bEnable;
		SC->bCaptureOnMovement = false;

		if (bEnable)
		{
			// 켤 때 품질/비용 세팅
			SC->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

			FEngineShowFlags& SF = SC->ShowFlags;
			SF.SetBloom(false);
			SF.SetMotionBlur(false);
			SF.SetVolumetricFog(false);
			SF.SetAmbientOcclusion(false);
			SF.SetSeparateTranslucency(false);
			SF.SetSubsurfaceScattering(false);
		}
	}
}

void AASequencePlayer::SetCinematicActive(bool bActive)
{
	// 서버면 리턴
	if (!HasAuthority())
		return;

	// 1) 모든 클라에 "메시 숨김/복구" 전파
	Multicast_SetActorsHidden(bActive);

	// 2) 각 로컬 클라에서 입력/HUD/카메라 제어 비활성 토글
	//    (SetCinematicMode는 "로컬 컨트롤러" 기준이므로 Client RPC로 각자 적용)
	Client_ApplyCinematicMode(bActive);
}


void AASequencePlayer::Multicast_SetActorsHidden_Implementation(bool bHiddenActor)
{
	UWorld* World = GetWorld();
	if (!World) return;

	// 시민 숨기기
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(World, APeopleBase::StaticClass(), Found);
		for (AActor* A : Found)
		{
			A->SetActorHiddenInGame(bHiddenActor);
		}
	}

	// 소방관 숨기기
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(World, AFireMan::StaticClass(), Found);
		for (AActor* A : Found)
		{
			A->SetActorHiddenInGame(bHiddenActor);
		}
	}

	// 마스크 숨기기
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(World, AMask::StaticClass(), Found);
		for (AActor* A : Found)
		{
			A->SetActorHiddenInGame(bHiddenActor);
		}
	}
	
}

void AASequencePlayer::Client_ApplyCinematicMode_Implementation(bool bEnable)
{
	// 각 클라 로컬에서만 적용됨(입력/HUD/카메라 제어)
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetCinematicMode(
			bEnable, // bInCinematicMode
			true,    // bHidePlayer(로컬 기준 숨김) — 이미 Multicast로 숨겼지만 중복 무해
			true,    // bAffectsHUD
			true,    // bDisableMovement
			true     // bDisableTurning
		);
	}
}


void AASequencePlayer::Skip()
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


void AASequencePlayer::MultiCast_Skip_Implementation()
{
	Skip();
}




