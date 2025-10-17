// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"



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

