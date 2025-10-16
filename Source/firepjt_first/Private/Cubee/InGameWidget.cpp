// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/InGameWidget.h"

#include "Components/TextBlock.h"
#include "Cubee/HouseGameState.h"
#include "Kismet/GameplayStatics.h"

void UInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HouseGameState = Cast<AHouseGameState>(UGameplayStatics::GetGameState(this));
}

void UInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateTimer();
}

void UInGameWidget::UpdateTimer()
{
	if (!HouseGameState) return;

	if (Txt_Timer)
	{
		FString TimerText = HouseGameState->GetFormattedTimer();
		Txt_Timer->SetText(FText::FromString(TimerText));
	}
}
