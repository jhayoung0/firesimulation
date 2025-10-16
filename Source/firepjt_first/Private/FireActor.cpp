// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActor.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"


// Sets default values
AFireActor::AFireActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRootComp);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->InitSphereRadius(55.f);
	SphereCollision->SetRelativeLocation(FVector(0, 0, 20));
	SphereCollision->SetCollisionProfileName(FName("Fire"));

	FireComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireComp"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/CustomContents/Fireman/Fire/NS_Fire.NS_Fire'"));
	if (FireRef.Succeeded())
	{
		FireComp->SetAsset(FireRef.Object);
	}
	FireComp->SetupAttachment(RootComponent);
	
}

void AFireActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

