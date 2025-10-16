// Fill out your copyright notice in the Description page of Project Settings.


#include "FireHose.h"

#include "FireActor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SkeletalMeshComponent.h"

#define FirehoseWater ECC_GameTraceChannel3

// Sets default values
AFireHose::AFireHose()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRootComp);

	FirehoseComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirehoseComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> FirehoseMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/CustomContents/Fireman/Water/firefighter_vision_fire_nozzle_turbo/StaticMeshes/SKM_firefighter_vision_fire_nozzle_turbo.SKM_firefighter_vision_fire_nozzle_turbo'"));
	if (FirehoseMeshRef.Succeeded())
	{
		FirehoseComp->SetSkeletalMesh(FirehoseMeshRef.Object);
	}
	FirehoseComp->SetupAttachment(RootComponent);

	NiagaraParticleSystemComp = CreateDefaultSubobject<UNiagaraComponent>("ParticleSystemComponent");
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraCompRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/CustomContents/Fireman/Water/NS_Water.NS_Water'"));
	if (niagaraCompRef.Succeeded())
	{
		NiagaraParticleSystemComp->SetAsset(niagaraCompRef.Object);
	}
	NiagaraParticleSystemComp->SetupAttachment(FirehoseComp);
	NiagaraParticleSystemComp->SetRelativeLocation(FVector(20, 0, 0));
	NiagaraParticleSystemComp->SetRelativeRotation(FRotator(-90, 0, 0));
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
		bool bHit = GetWorld()->SweepSingleByChannel(
			hitResult,
			data.Position,
			end,
			FQuat::Identity,
			FirehoseWater,
			FCollisionShape::MakeSphere(15),
			params
		);
		DrawDebugSphere(GetWorld(), data.Position, 15, 1, FColor::Red);
		// UE_LOG(LogTemp, Warning, TEXT("bHit가 될 것인가 %d"), bHit);
		
		if (bHit)
		{
			if (AFireActor* fire = Cast<AFireActor>(hitResult.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *fire->GetName());
			}
		}
	}
}