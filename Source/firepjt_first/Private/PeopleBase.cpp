// Fill out your copyright notice in the Description page of Project Settings.


#include "PeopleBase.h"
#include "EnhancedInputComponent.h"

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




// c 키 
void APeopleBase::crawlAction()
{
	if (IsCrawl)
	{
		// 플레이어가 기고 있으면
		
	}
}


// e 키
void APeopleBase::Interaction()
{
}
