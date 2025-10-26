// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/GameOverWidget.h"

#include "Components/Button.h"
#include "Cubee/HouseGameMode.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Back)
	{
		Btn_Back->OnClicked.AddDynamic(this, &UGameOverWidget::OnBackButtonClicked);
	}
}

void UGameOverWidget::OnBackButtonClicked()
{
	
}
