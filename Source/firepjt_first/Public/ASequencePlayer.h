// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CinematicSC.h"
#include "CinematicUI.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "MainUI.h"
#include "PhoneWidget.h"
#include "GameFramework/Actor.h"
#include "ASequencePlayer.generated.h"

class ASceneCapture2D;
class UTextureRenderTarget2D;

class ULevelSequencePlayer;

UCLASS()
class FIREPJT_FIRST_API AASequencePlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AASequencePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 모든 시퀀스 시작 및 종료 시 호출
	UFUNCTION()
	void SequencePlay();
	UFUNCTION()
	void SequenceEnd();

	
	// 시네마틱 플레이 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPlayingCinematic;

	// intro cinematic
	void PlayIntroSequence();
	
	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> FirstSequence;
	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> SecondSequence;
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> FirstPlayer;
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SecondPlayer;
	UFUNCTION()
	void OnFirstSequenceFinished();
	UFUNCTION()
	void OnSecondSequenceFinished();
	

	// 미션 1 CINEMATIC
	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> Mission1_People_Sequence;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> Mission1_Fireman_Sequence;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> Mission1_People_Player;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> Mission1_Fireman_Player;

	UFUNCTION()
	void MissionOneSequencePlay();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_MissionOneSequencePlay();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_MissionOneSequencePlay();
	
	// 델리게이트
	UFUNCTION(BlueprintCallable, Category="Cinematic")
	void BindToWidget(UPhoneWidget* InWidget);

	

	// 미션 2 시네마틱
	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> Mission2_People_Sequence;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> Mission2_Fireman_Sequence;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> Mission2_People_Player;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> Mission2_Fireman_Player;

	UFUNCTION()
	void MissionTwoSequencePlay();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_MissionTwoSequencePlay();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_MissionTwoSequencePlay();

	// 미션 3 시네마틱
	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> Mission3_Sequence;
;
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> Mission3_Player;
	
	UFUNCTION()
	void MissionThreeSequencePlay();
	
		
	UFUNCTION(Server, Reliable)
	void ServerRPC_MissionThreeSequencePlay();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_MissionThreeSequencePlay();

	UFUNCTION()
	void LastSequenceFinished();


public:
	// 시퀀스 플레이어
	UPROPERTY(EditDefaultsOnly)
	class Afirepjt_firstPlayerController* FirePC;
	
public: // widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UCinematicUI> cinematicwidget;

	UPROPERTY(Transient)
	UCinematicUI* cinematicUI = nullptr;
	
public: // scene capture actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =Camera)
	ACinematicSC* leftSC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =Camera)
	ACinematicSC* rightSC;

	// 씬 캡쳐 온 오프
	UFUNCTION()
	void SetCaptureActive(ACinematicSC* CaptureActor, bool bEnable);

	// 시네마틱에서 액터들 숨기기
public:
	UFUNCTION(BlueprintCallable)
	void SetCinematicActive(bool bActive);
private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetActorsHidden(bool bHiddenActor);

	UFUNCTION(Client, Reliable)
	void Client_ApplyCinematicMode(bool bEnable);
	
public: // 스킵
	void Skip();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_Skip();

public: // sfx
	
	UPROPERTY()
	class UAudioComponent* BGMComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sfx)
	class USoundBase* BGM_Sound;
	
};
