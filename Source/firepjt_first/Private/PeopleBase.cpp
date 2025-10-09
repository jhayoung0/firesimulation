// Fill out your copyright notice in the Description page of Project Settings.


#include "PeopleBase.h"
#include "EnhancedInputComponent.h"
#include "InteractActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "MainUI.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

APeopleBase::APeopleBase()
{
	//카메라 초기 위치 설정
	GetFirstPersonCameraComponent()->SetRelativeLocation(CameraLocation);
	GetFirstPersonCameraComponent()->SetRelativeRotation(CamerRotation);

	//상호작용 액터 붙일 컴포넌트
	compActor = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingActor"));
	compActor ->SetupAttachment(GetMesh(), TEXT("hand_r"));
	compActor->SetRelativeLocation(FVector(0,7,5.5f));
}

void APeopleBase::BeginPlay()
{
	Super::BeginPlay();

	// 레벨에 있는 모든 상호작용 액터를 찾자.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractActor::StaticClass(), allInteractActor);


	// Main ui를 만들자
	if (mainwidget)
	{
		mainui = CreateWidget<UMainUI>(GetWorld(), mainwidget);
		if (mainui)
		{
			mainui->AddToViewport();
			currOxygen = maxOxygen;
			const float InitPercent = maxOxygen > 0.f ? currOxygen / maxOxygen : 0.f;
			mainui->SetOxygenPercent(InitPercent);
		}
	}


}


void APeopleBase::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput)
	{
		PlayerInput->BindAction(crawlInput, ETriggerEvent::Started, this, &APeopleBase::crawlAction);
		PlayerInput->BindAction(InteractionInput, ETriggerEvent::Started, this, &APeopleBase::Interaction);
	}
}

void APeopleBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 상태별 계수 먼저 갱신
	Posture = IsCrawl ? 0.8f : 1.0f;

	if (HasMask) {
		Gear = 0.4f;
	}
	else if (HasWetTowel) {
		Gear = 0.95f;
	}
	else {
		Gear = 1.0f;
	}

	if (FMath::Abs(Gear - PreviousGear) > KINDA_SMALL_NUMBER || HasMask != bPreviousHasMask || HasWetTowel != bPreviousHasWetTowel)
	{
		UE_LOG(LogTemp, Log, TEXT("Oxygen gear updated - Gear: %.2f, HasMask: %s, HasWetTowel: %s"),
			Gear,
			HasMask ? TEXT("true") : TEXT("false"),
			HasWetTowel ? TEXT("true") : TEXT("false"));
		PreviousGear = Gear;
		bPreviousHasMask = HasMask;
		bPreviousHasWetTowel = HasWetTowel;
	}

	// 산소 소모 계산
	currOxygen -= BaseRate * Posture * Gear * DeltaSeconds;

	// 0 이하 방지
	currOxygen = FMath::Clamp(currOxygen, 0.0f, maxOxygen);

	const float percent = maxOxygen > 0.f ? currOxygen / maxOxygen : 0.f;

	// 프로그레스 바 업뎃
	if (mainui)
	{
		mainui->SetOxygenPercent(percent);
	}
}

void APeopleBase::ApplyCrawlState(bool bEnable)
{

	constexpr float StandHalf = 88.f;
	constexpr float CrawlHalf = 48.f;
	GetCapsuleComponent()->SetCapsuleHalfHeight(bEnable ? CrawlHalf : StandHalf);

	// crawl 상태일 때는 더 느리게
	GetCharacterMovement()->MaxWalkSpeed = bEnable ? 200.f : 600.f;


}


// c 키 
void APeopleBase::crawlAction()
{
	if (IsCrawl)
	{
		// 기고 있는 상태 → 일어나기
		IsCrawl = false;

		
		ApplyCrawlState(IsCrawl);
		GetFirstPersonCameraComponent()->SetRelativeLocation(CameraLocation);
		GetFirstPersonCameraComponent()->SetRelativeRotation(CamerRotation);
		// mesh 높이 조정
		GetMesh()->SetRelativeLocation(FVector(0.000000,0.000000,-95.000000));
	}
	else
	{
		// 서 있는 상태 → 기기 시작
		IsCrawl = true;
		
		ApplyCrawlState(IsCrawl);
		GetFirstPersonCameraComponent()->SetRelativeLocation(CameramLocationCrawl);
		GetFirstPersonCameraComponent()->SetRelativeRotation(CamerRotationCrawl);

		// mesh 높이 조정
		GetMesh()->SetRelativeLocation(FVector(0.000000,0.000000,-45.000000));
	}
}


// e 키
void APeopleBase::Interaction()
{

	if (IsInteracting)
	{
		AInteractActor* tempActor = InteractingActor;
		DetachActor(tempActor);
		return;
	}

	
	// 거리가 가까운 상호작용 가능 물체를 변수에 담고
	// 가장 거리가 가까운 상호작용 가능 물체의 상호작용 함수를 호출해준다.
	if (allInteractActor.Num() == 0)
	{
		return;
	}

	float MinDist = CanInteractDist; // 어차피 이 거리보다 크면 체크안하니까 
	float ActorDist = 0.f;
	int32 ClosestIndex = -1;
	for (int32 i = 0; i < allInteractActor.Num(); i++) 
	{
		ActorDist	=  (GetActorLocation() - allInteractActor[i]->GetActorLocation()).Length();
		if (ActorDist < MinDist)
		{
			MinDist = ActorDist;
			ClosestIndex = i;
		}
	}
	
	if (ClosestIndex != -1) {
		InteractingActor = Cast<AInteractActor>(allInteractActor[ClosestIndex]);
		// 검색된 액터의 상호작용 함수를 호출
		if (InteractingActor) {
			InteractingActor->PlayInteract();
			AttachActor();
		}
	}
	
}

void APeopleBase::AttachActor()
{
	// compActor에 붙이자.
	InteractingActor->AttachToComponent(compActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	IsInteracting = true;
	InteractingActor->IsInteracting = true;

	// tag에 따라서 구분하자.
	if (InteractingActor)
	{
		if (InteractingActor->ActorHasTag(FName("Mask")))
		{
		UE_LOG(LogTemp, Log, TEXT("mask!"));
		HasMask = true;
		HasWetTowel = false;
	}
	else if (InteractingActor->ActorHasTag(FName("WetTowel")))
	{
		UE_LOG(LogTemp, Log, TEXT("WetTowel!"));
		HasWetTowel = true;
		HasMask = false;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("태그 없음 또는 알 수 없는 타입"));
		}
	}

}

void APeopleBase::DetachActor(AInteractActor* tempActor)
{
	IsInteracting = false;
	tempActor->IsInteracting = false;
	// 분리하자
	if (tempActor)
	{
		tempActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	

	// tag에 따라서 구분하자.
	if (tempActor)
	{
		if (tempActor->ActorHasTag(FName("Mask")))
		{
			UE_LOG(LogTemp, Log, TEXT("mask!"));
			HasMask = false;
		}
		else if (tempActor->ActorHasTag(FName("WetTowel")))
		{
			UE_LOG(LogTemp, Log, TEXT("WetTowel!"));
			HasWetTowel = false;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("태그 없음 또는 알 수 없는 타입"));
		}
	}

	InteractingActor = nullptr;
}



