// Fill out your copyright notice in the Description page of Project Settings.


#include "PeopleBase.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APeopleBase::APeopleBase()
{
	//카메라 초기 위치 설정
	GetFirstPersonCameraComponent()->SetRelativeLocation(CameraLocation);
	GetFirstPersonCameraComponent()->SetRelativeRotation(CamerRotation);
}

void APeopleBase::BeginPlay()
{
	Super::BeginPlay();

	
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

void APeopleBase::ApplyCrawlState(bool bEnable)
{

	constexpr float StandHalf = 88.f;
	constexpr float CrawlHalf = 48.f;
	GetCapsuleComponent()->SetCapsuleHalfHeight(bEnable ? CrawlHalf : StandHalf);

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
	}
	else
	{
		// 서 있는 상태 → 기기 시작
		IsCrawl = true;
		ApplyCrawlState(IsCrawl);
		GetFirstPersonCameraComponent()->SetRelativeLocation(CameramLocationCrawl);
		GetFirstPersonCameraComponent()->SetRelativeRotation(CamerRotationCrawl);
	}
}


// e 키
void APeopleBase::Interaction()
{
	// 거리가 가까운 상호작용 가능 물체를 변수에 담고
	
	// 가장 거리가 가까운 상호작용 가능 물체의 상호작용 함수를 호출해준다.
	
	
}



