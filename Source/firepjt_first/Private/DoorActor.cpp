// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"

#include "InteractWidgetComp.h"
#include "Components/BoxComponent.h"


// Sets default values
ADoorActor::ADoorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Box Comp
	boxComp->SetBoxExtent(FVector(10,50,100));
	boxComp->SetSimulatePhysics(false);

	// Mesh Comp
	meshComp->SetRelativeLocation(FVector(0, 46, -103));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> doorMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/StarterContent/Props/SKM_Door.SKM_Door'"));
	if (doorMeshRef.Succeeded())
	{
		meshComp->SetSkeletalMesh(doorMeshRef.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> element1Ref(TEXT("/Script/Engine.Material'/Game/CustomContents/House/Assets/Apartments/main_door_back.main_door_back'"));
	if (element1Ref.Succeeded())
	{
		meshComp->SetMaterial(0, element1Ref.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> element2Ref(TEXT("/Script/Engine.Material'/Game/CustomContents/House/Assets/Apartments/metal_black.metal_black'"));
	if (element2Ref.Succeeded())
	{
		meshComp->SetMaterial(1, element2Ref.Object);
	}

	// Interact Widget Comp
	InteractWidgetComp->SetRelativeLocation(FVector(56,-14,-7));
	InteractWidgetComp->SetRelativeScale3D(FVector(0.5f));

	// Door Break Animation Pos
	animationPos = CreateDefaultSubobject<USceneComponent>(TEXT("AnimationPos"));
	animationPos->SetRelativeLocation(FVector(90,-25,-134));
	animationPos->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ADoorActor::GetAnimPos()
{
	return animationPos->GetComponentLocation();
}

