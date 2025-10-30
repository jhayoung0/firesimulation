// Fill out your copyright notice in the Description page of Project Settings.


#include "Mask.h"

#include "Components/BoxComponent.h"


// Sets default values
AMask::AMask()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 붙이기
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(boxComp);
	meshComp->SetupAttachment(RootComponent);

	// 콜리전 설정
	boxComp->SetCollisionProfileName(FName("InteractActor"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// simulate physics
	boxComp->SetSimulatePhysics(true);
	
}


// Called when the game starts or when spawned
void AMask::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMask::ToggleMask(bool takeon) 
{
	if (takeon)
	{
		boxComp->SetSimulatePhysics(false);
	}
	else
	{
		boxComp->SetSimulatePhysics(true);
	}
}

