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

void AFireActor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, DebrisRate))
	{
		FireComp->SetVariableFloat(FName("DebrisRate"), DebrisRate);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, FlameSpawnRate))
	{
		FireComp->SetVariableFloat(FName("FlameSpawnRate"), FlameSpawnRate);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, FlameRadius))
	{
		FireComp->SetVariableFloat(FName("FlameRadius"), FlameRadius);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, FlameVelocity))
	{
		FireComp->SetVariableFloat(FName("FlameVelocity"), FlameVelocity);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, FlameConeAngle))
	{
		FireComp->SetVariableFloat(FName("FlameConeAngle"), FlameConeAngle);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, SmokeSpriteSizeMin))
	{
		FireComp->SetVariableFloat(FName("SmokeSpriteSizeMin"), SmokeSpriteSizeMin);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, SmokeSpriteSizeMax))
	{
		FireComp->SetVariableFloat(FName("SmokeSpriteSizeMax"), SmokeSpriteSizeMax);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, SmokeLifetime))
	{
		FireComp->SetVariableFloat(FName("SmokeLifetime"), SmokeLifetime);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, DebrisVelocity))
	{
		FireComp->SetVariableFloat(FName("DebrisVelocity"), DebrisVelocity);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFireActor, DebrisConeAngle))
	{
		FireComp->SetVariableFloat(FName("DebrisConeAngle"), DebrisConeAngle);
	}
}

void AFireActor::PutOutFire()
{
	// Use Alpha Value for synchronizing variable Scale Rates 
	FlameScale = FMath::FInterpConstantTo(FlameScale, 0.f, GetWorld()->DeltaTimeSeconds, vanishAlpha);
	SmokeScale = FMath::FInterpConstantTo(SmokeScale, 0.f, GetWorld()->DeltaTimeSeconds, vanishAlpha);;
	DistortionScale = FMath::FInterpConstantTo(DistortionScale, 0.f, GetWorld()->DeltaTimeSeconds, vanishAlpha);
	DebrisRate = FMath::FInterpConstantTo(DebrisRate, 0.f, GetWorld()->DeltaTimeSeconds, vanishAlpha);;
	
	FireComp->SetVariableFloat(FName("FlameScale"), FlameScale);
	FireComp->SetVariableFloat(FName("SmokeScale"), SmokeScale);
	FireComp->SetVariableFloat(FName("DistortionScale"), DistortionScale);
	FireComp->SetVariableFloat(FName("DebrisRate"), DebrisRate);

	UE_LOG(LogTemp, Warning, TEXT("FlameScale : %f"), FlameScale);
	if (FlameScale <= 0.001f)
	{
		Destroy();
	}
}






