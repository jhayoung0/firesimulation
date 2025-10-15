// Fill out your copyright notice in the Description page of Project Settings.


#include "FireHose.h"

#include "FireActor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Engine/OverlapResult.h"


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

	NiagaraParticleSystemComp->SetActorParameter(FName("UserCallbackHandler"), this);
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

	FVector offset = FVector(1, 1, 1);

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	
	for (const auto& data : Data)
	{
		FVector start = data.Position;
		FVector end = start + offset;
		
		FHitResult hitResult;
		bool bHit = GetWorld()->SweepSingleByProfile(
			hitResult, data.Position, end, FQuat::Identity, FName("Fire"), FCollisionShape::MakeSphere(data.Size), params);
		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *start.ToString());
		}
	}
}