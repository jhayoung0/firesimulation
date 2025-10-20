// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CinematicUI.h"
#include "LevelSequence.h"
#include "MainUI.h"
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


	// 시네마틱 플레이 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPlayingCinematic;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> FirstSequence;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> SecondSequence;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> FirstPlayer;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SecondPlayer;

	// 미션 1 CINEMATIC
	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> Mission1_People_Sequence;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> Mission1_Fireman_Sequence;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> Mission1_People_Player;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> Mission1_Fireman_Player;

	void PlayIntroSequence();

	// 미션 2 ..


	
	UFUNCTION()
	void OnFirstSequenceFinished();
	UFUNCTION()
	void OnSecondSequenceFinished();



public: // widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UCinematicUI> cinematicwidget;

	UPROPERTY(Transient)
	UCinematicUI* cinematicUI = nullptr;


public: // 스킵
	UFUNCTION()
	void DoSkip();

	
};
