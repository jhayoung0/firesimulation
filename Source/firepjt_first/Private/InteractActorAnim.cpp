// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActorAnim.h"

#include "InteractActor.h"

void UInteractActorAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	pawnOwner = Cast<AInteractActor>(TryGetPawnOwner());
}

void UInteractActorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	Isinteracting = pawnOwner->IsInteracting;
}
