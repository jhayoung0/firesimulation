// Fill out your copyright notice in the Description page of Project Settings.


#include "Crowbar.h"

// Sets default values
ACrowbar::ACrowbar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	SetRootComponent(SceneRootComponent);
	
	CrowbarMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CrowbarMeshComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> crowbarMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Fab/Crowbar_Low-poly/crowbar/StaticMeshes/SKM_crowbar.SKM_crowbar'"));
	if (crowbarMeshRef.Succeeded())
	{
		CrowbarMeshComp->SetSkeletalMesh(crowbarMeshRef.Object);
	}
	CrowbarMeshComp->SetupAttachment(SceneRootComponent);
}

// Called when the game starts or when spawned
void ACrowbar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrowbar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

