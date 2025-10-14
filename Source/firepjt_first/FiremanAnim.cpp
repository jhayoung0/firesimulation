// Fill out your copyright notice in the Description page of Project Settings.


#include "FiremanAnim.h"
#include "Fireman.h"

void UFiremanAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	pawnOwner = Cast<AFireMan>(TryGetPawnOwner());
}

void UFiremanAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!pawnOwner)
	{
		return;
	}

	Velocity = pawnOwner->GetVelocity();
	Horizontal = FVector::DotProduct(pawnOwner->GetActorRightVector(), Velocity);
	Vertical = FVector::DotProduct(pawnOwner->GetActorForwardVector(), Velocity);
}
