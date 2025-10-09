// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/InfoWidget.h"

#include "Components/TextBlock.h"


void UInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeTextBlocks();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInfoWidget::SetInfoDataAsset(UInfoDataAsset* NewDataAsset)
{
	InfoDataAsset = NewDataAsset;
}

void UInfoWidget::ViewInfoWidget(int32 Idx)
{
	InitializeTextBlocks();
	SetVisibility(ESlateVisibility::Visible);

	SetInfoTextFromIndex(Idx);

	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}

void UInfoWidget::RemoveInfoWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInfoWidget::SetInfoTextFromIndex(int32 Index)
{
	if (!InfoDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("No DataAsset found"));
		return;
	}

	if (!InfoDataAsset->InfoTexts.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Index Value"));
		return;
	}

	const FInfoTextEntry Entry = InfoDataAsset->InfoTexts[Index];

	Txt_Title->SetText(Entry.Title);
	Txt_Description->SetText(Entry.Description);
}

void UInfoWidget::InitializeTextBlocks()
{
	Txt_Title->SetText(FText::GetEmpty());
	Txt_Description->SetText(FText::GetEmpty());
}
