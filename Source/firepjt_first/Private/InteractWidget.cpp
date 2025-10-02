// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Components/TextBlock.h"

void UInteractWidget::SetInteractText(FString key, FString desc)
{
	interactKey->SetText(FText::FromString(key));
	description->SetText(FText::FromString(desc));
}
