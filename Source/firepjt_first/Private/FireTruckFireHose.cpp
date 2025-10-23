// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTruckFireHose.h"


// Sets default values
AFireTruckFireHose::AFireTruckFireHose()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FireHoseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireHoseComp"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> fireHoseRef(TEXT("/Script/Engine.StaticMesh'/Game/CustomContents/Fireman/FireTruck/emergency_fire_hose_9_mb/StaticMeshes/emergency_fire_hose_9_mb.emergency_fire_hose_9_mb'"));
	if (fireHoseRef.Succeeded())
	{
		FireHoseComp->SetStaticMesh(fireHoseRef.Object);
	}
}

// Called when the game starts or when spawned
void AFireTruckFireHose::BeginPlay()
{
	Super::BeginPlay();
	
}

