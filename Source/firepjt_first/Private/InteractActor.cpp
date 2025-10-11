// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActor.h"

#include "InteractWidget.h"
#include "InteractWidgetComp.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values
AInteractActor::AInteractActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(meshComp);


	InteractWidgetComp = CreateDefaultSubobject<UInteractWidgetComp>(TEXT("InteractWidget"));
	InteractWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	InteractWidgetComp->SetupAttachment(RootComponent);
	InteractWidgetComp->SetTwoSided(true);

}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();

	if (UUserWidget* W = InteractWidgetComp->GetWidget())
	{
		InteractUI = Cast<UInteractWidget>(W);
		if (InteractUI)
		{
			InteractUI->SetVisibility(ESlateVisibility::Visible); 
		}
	}
}


// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractActor::PlayInteract()
{
	// 인터랙트 몽타주 재생
	UAnimInstance* anim = meshComp->GetAnimInstance();
	
	if (anim && InteractMontage)
	{
		anim->Montage_Play(InteractMontage);
		
	}

	// 상호작용 표시 끄기

	
}

void AInteractActor::ToggleWidget(bool check)
{
	if (check)
	{
		InteractUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractUI->SetVisibility(ESlateVisibility::Hidden);
	}
	
	
}

