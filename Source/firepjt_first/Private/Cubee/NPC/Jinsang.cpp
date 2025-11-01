// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/NPC/Jinsang.h"
#include "PeopleBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AJinsang::EndDialogue()
{
	Super::EndDialogue();

	IsOnceTalked = true;

	
	APeopleBase* people = Cast<APeopleBase>(UGameplayStatics::GetActorOfClass(GetWorld(), APeopleBase::StaticClass()));
	if (people)
	{
		people->AttachJinsang();
	}

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	//GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	

	
}

bool AJinsang::IsWalking()
{
	if (IsOnceTalked && Cast<APeopleBase>(GetAttachParentActor()))
	{
		return GetAttachParentActor()->GetVelocity().Size() > 0.f;
	}
	
	return true;
}

