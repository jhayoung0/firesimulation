// Fill out your copyright notice in the Description page of Project Settings.


#include "FireMan.h"


// Sets default values
AFireMan::AFireMan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ConstructorHelpers::FObjectFinder<USkeletalMeshComponent>
}

// Called when the game starts or when spawned
void AFireMan::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFireMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

