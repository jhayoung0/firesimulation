// Fill out your copyright notice in the Description page of Project Settings.


#include "FiremanMainUI.h"
#include "Components/SizeBox.h"
#include "Cubee/InfoWidget.h"


UFiremanMainUI::UFiremanMainUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UInfoWidget> infoWidgetRef(TEXT("/Game/CustomContents/House/UI/WBP_Info.WBP_Info_C"));
	if (infoWidgetRef.Succeeded())
	{
		InfoWidgetClass = infoWidgetRef.Class;
	}

	ConstructorHelpers::FObjectFinder<UInfoDataAsset> daFiremanRef(TEXT("/Script/firepjt_first.InfoDataAsset'/Game/CustomContents/House/DA_Firefighter.DA_Firefighter'"));
	if (daFiremanRef.Succeeded())
	{
		DAFireman = daFiremanRef.Object;
	}
}

void UFiremanMainUI::AddInfoUI(int32 idx)
{
	// Info UI
	UInfoWidget* info = CreateWidget<UInfoWidget>(GetWorld(), InfoWidgetClass);
	info->SetInfoDataAsset(DAFireman);

	// add to size box
	SizeBox_Info->SetContent(info);
	// view info widget
	info->ViewInfoWidget(idx);
}