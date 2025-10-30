// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActor.h"

#include "InteractWidget.h"
#include "InteractWidgetComp.h"
#include "MainUI.h"
#include "PeopleBase.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AInteractActor::AInteractActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp= CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp")); 
	meshComp->SetupAttachment(RootComponent);

	InteractWidgetComp = CreateDefaultSubobject<UInteractWidgetComp>(TEXT("InteractWidget"));
	InteractWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	InteractWidgetComp->SetupAttachment(RootComponent);
	InteractWidgetComp->SetTwoSided(true);

	// 아웃라인 설정
	meshComp->bRenderCustomDepth = true;

	// 콜리전 설정
	boxComp->SetCollisionProfileName(FName("InteractActor"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// simulate physics
	boxComp->SetSimulatePhysics(true);
	InteractWidgetComp->SetCollisionProfileName(FName("UI"));


	// replicates 켜주기
	bReplicates = true;
}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
	if (UUserWidget* W = InteractWidgetComp->GetWidget())
	{
		InteractUI = Cast<UInteractWidget>(W);
		if (InteractUI)
		{
			InteractUI->SetVisibility(ESlateVisibility::Visible); 
		}
	}

	// 콜리젼 이벤트 바인딩
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractActor::OnBoxBeginOverlap);
	

}


// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BillboardInteractKey();
}


// 네트워크 설정
void AInteractActor::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate 할 변수 등록
	// DOREPLIFETIME(AInteractActor, IsInteracting);
}


void AInteractActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!ActorHasTag(FName("People"))) {return;}
	if (!OtherActor || !OtherComp) return;
	if (!OtherActor->ActorHasTag(FName("ThreeSubMission"))) return;

	APeopleBase* player = Cast<APeopleBase>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// 다음 미션으로 넘기기
	if (player && player->mainui->CurrentSubMission == 3)
	{
		player->mainui->SuccessSubMission();
	}

}

void AInteractActor::ToggleWidget(bool check)
{
	if (check)
	{
		// detach 되었을 때
		InteractUI->SetVisibility(ESlateVisibility::Visible);
		if (meshComp)
		{
			// AnimBP를 거치지 않고 단일 애니메이션 모드로 전환
			meshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			// 루프 재생
			meshComp->PlayAnimation(InteractAnim_Detach, true);
			meshComp->SetRenderCustomDepth(true);
			boxComp->SetSimulatePhysics(true);
		}
	}
	else
	{
		// attach 되었을 때
		InteractUI->SetVisibility(ESlateVisibility::Hidden);
		if (meshComp)
		{
			// AnimBP를 거치지 않고 단일 애니메이션 모드로 전환
			meshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			// 루프 재생
			meshComp->PlayAnimation(InteractAnim, true);
			meshComp->SetRenderCustomDepth(false);
			boxComp->SetSimulatePhysics(false);
		}
	}
}



void AInteractActor::BillboardInteractKey()
{
	
	// 내가 컨트롤하고 있는 카메라를 가져오자.
	AActor* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// 카메라의 앞 방향 (반대), 윗 방향을 이용해서 Rotator 를 구하자.
	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-cam->GetActorForwardVector(), cam->GetActorUpVector());
	// 구한 Rotator 를 comHP 에 설정
	InteractWidgetComp->SetWorldRotation(rot); 
	
	
}


