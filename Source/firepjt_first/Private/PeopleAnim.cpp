// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PeopleAnim.h"
#include "PeopleBase.h"
#include "GameFramework/PawnMovementComponent.h"

void UPeopleAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 소유 PAWN 설정
	pawnOwner = Cast<APeopleBase>(TryGetPawnOwner());
	
}

void UPeopleAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!pawnOwner) {return;}
	// 이동 속도 설정
	velocity = pawnOwner->GetVelocity();
	// 수평 이동 속력
	groundSpeed = velocity.Size2D();
	// 공중 여부 설정
	isinAir = pawnOwner->GetMovementComponent()->IsFalling();
	// 기고 있는지 여부 설정
	isCrawl = pawnOwner->IsCrawl;
	// 젖은 타월 쓰고 있는지 여부 설정
	HasWetTowel = pawnOwner->HasWetTowel;
	// 사람 옮기고 있는지 여부 설정
	CarryingPeople = pawnOwner->RescuePeople;
	// 폰 사용 중인지 여부 설정
	HasPhone = pawnOwner->HasPhone;
	
	// 앞 뒤 이동에 따른 값
	FVector v = velocity;
	v.Z = 0;
	dirH = FVector::DotProduct(pawnOwner->GetActorRightVector(),v);
	dirV = FVector::DotProduct(pawnOwner->GetActorForwardVector(), v);
}
