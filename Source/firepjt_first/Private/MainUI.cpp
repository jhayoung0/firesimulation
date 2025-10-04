// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/ProgressBar.h"

void UMainUI::SetOxygenPercent(float percent)
{
	OxygenBar->SetPercent(percent);
}
