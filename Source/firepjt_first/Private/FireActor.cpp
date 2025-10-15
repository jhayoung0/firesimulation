// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActor.h"


// Sets default values
AFireActor::AFireActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRootComp);
}

void AFireActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

