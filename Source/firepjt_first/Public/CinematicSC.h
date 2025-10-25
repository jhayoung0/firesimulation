// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "CinematicSC.generated.h"

UCLASS()
class FIREPJT_FIRST_API ACinematicSC : public ASceneCapture2D
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACinematicSC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
