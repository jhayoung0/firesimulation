// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTruckCrowbar.h"

#include "InteractWidgetComp.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AFireTruckCrowbar::AFireTruckCrowbar()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CrowbarComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrowbarComp"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> CrowbarRef(TEXT("/Script/Engine.StaticMesh'/Game/Fab/Crowbar_Low-poly/crowbar/StaticMeshes/crowbar.crowbar'"));
	if (CrowbarRef.Succeeded())
	{
		CrowbarComp->SetStaticMesh(CrowbarRef.Object);
	}
}

// Called when the game starts or when spawned
void AFireTruckCrowbar::BeginPlay()
{
	Super::BeginPlay();
	
}