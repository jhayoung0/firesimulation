// Fill out your copyright notice in the Description page of Project Settings.


#include "FireHose.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


// Sets default values
AFireHose::AFireHose()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRootComp);

	NiagaraParticleSystemComp = CreateDefaultSubobject<UNiagaraComponent>("ParticleSystemComponent");
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraCompRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/CustomContents/Fireman/Water/NS_Water.NS_Water'"));
	if (niagaraCompRef.Succeeded())
	{
		NiagaraParticleSystemComp->SetAsset(niagaraCompRef.Object);
	}
	NiagaraParticleSystemComp->SetupAttachment(SceneRootComp);
	NiagaraParticleSystemComp->SetAutoActivate(false);
}

void AFireHose::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFireHose::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireHose::OnWaterShot()
{
	NiagaraParticleSystemComp->SetAutoActivate(true);
}

void AFireHose::OnDirectShotMode()
{
}

void AFireHose::OnSprayShotMode()
{
}

void AFireHose::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);

	for (const auto& data : Data)
	{
		// print log 해야됨
		UE_LOG(LogTemp, Warning, TEXT("(%f, %f, %f)"), data.Position.X, data.Position.Y, data.Position.Z);
	}
}