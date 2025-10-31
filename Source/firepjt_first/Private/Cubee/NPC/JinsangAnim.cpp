// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/NPC/JinsangAnim.h"

#include "Cubee/NPC/Jinsang.h"

void UJinsangAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn= Cast<AJinsang>(TryGetPawnOwner());
}

void UJinsangAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwningPawn) return;

	IsTalking = OwningPawn->IsTalking;
	IsOnceTalked = OwningPawn->IsOnceTalked;
}
