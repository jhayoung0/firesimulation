// Fill out your copyright notice in the Description page of Project Settings.


#include "PeopleOnePC.h"

#include "PeopleBase.h"
#include "GameFramework/GameModeBase.h"


void APeopleOnePC::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn())
	{
		APeopleBase* P = GetWorld()->SpawnActor<APeopleBase>(GetWorld()->GetAuthGameMode()->DefaultPawnClass);
		if (P) { Possess(P); }
	}
	
}

void APeopleOnePC::OpenPhoneUI()
{
	// phone ui를 만들자
	if (phonewidget)
	{
		phoneUI = CreateWidget<UPhoneWidget>(GetWorld(), phonewidget);
		
	}
	
	phoneUI->AddToViewport();
	
	bShowMouseCursor = true;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	
	
	
}

void APeopleOnePC::ClosePhoneUI()
{
	phoneUI->RemoveFromParent();

	bShowMouseCursor = false;

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
}
