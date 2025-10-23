// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Cubee/InfoWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"


void UMainUI::AddInfoUI(int32 idx)
{
	// info widget 만들자 
	UInfoWidget* info = CreateWidget<UInfoWidget>(GetWorld(), InfoWidget);
	info->SetInfoDataAsset(peopleda);
	// size box에 추가
	//SizeBox_Info->AddChild(info);
	SizeBox_Info->SetContent(info);
	// view info widget
	info->ViewInfoWidget(idx); 
}



void UMainUI::ShowDamageUI(float opacity)
{
	DamageUI->SetRenderOpacity(opacity);
}

void UMainUI::ShowMaskUI(bool check)
{
	check? maskUIopacity = 0.3f : maskUIopacity = 0;
	OxygenMaskUI->SetRenderOpacity(maskUIopacity);
}

void UMainUI::SetOxygenPercent(float percent)
{
	OxygenBar->SetPercent(percent);
}

/*
void UMainUI::ToggleCinematic(bool Open)
{
	if (Open)
	{
		cinematicWBP->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		cinematicWBP->SetVisibility(ESlateVisibility::Hidden);
	}
}

*/