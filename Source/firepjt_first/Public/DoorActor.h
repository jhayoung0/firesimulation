// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractActor.h"
#include "DoorActor.generated.h"

UCLASS()
class FIREPJT_FIRST_API ADoorActor : public AInteractActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetAnimPos();

protected:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* animationPos;
};
