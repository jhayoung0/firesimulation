// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTruckFireHose.h"

#include "InteractWidgetComp.h"


// Sets default values
AFireTruckFireHose::AFireTruckFireHose()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FireHoseComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireHoseComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> fireHoseRef(TEXT("/Script/Engine.SkeletalMesh'/Game/CustomContents/Fireman/FireTruck/emergency_fire_hose_9_mb/StaticMeshes/SKM_emergency_fire_hose_9_mb.SKM_emergency_fire_hose_9_mb'"));
	if (fireHoseRef.Succeeded())
	{
		FireHoseComp->SetSkeletalMesh(fireHoseRef.Object);
	}

	// Interact Widget Comp
	InteractWidgetComp = CreateDefaultSubobject<UInteractWidgetComp>(TEXT("InteractWidgetComp"));
	ConstructorHelpers::FClassFinder<UUserWidget> interactWidgetCompRef(TEXT("/Game/CustomContents/UI/WBP_InteractWidget.WBP_InteractWidget_C"));
	if (interactWidgetCompRef.Succeeded())
	{
		InteractWidgetComp->SetWidgetClass(interactWidgetCompRef.Class);
		InteractWidgetComp->SetupAttachment(FireHoseComp);
		InteractWidgetComp->SetRelativeLocationAndRotation(FVector(0,40,-70), FRotator(0,180,0));
	}
}

void AFireTruckFireHose::HideInteractWidget()
{
	InteractWidgetComp->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
}

// Called when the game starts or when spawned
void AFireTruckFireHose::BeginPlay()
{
	Super::BeginPlay();
	
}

