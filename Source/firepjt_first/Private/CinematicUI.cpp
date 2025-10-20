// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicUI.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"

void UCinematicUI::OpenWidgetToggle(bool isdual)
{
	// dual 화면이면
	if (isdual)
	{
		sizebox_one_People->SetRenderOpacity(0.0f);
		sizebox_one_Fireman->SetRenderOpacity(0.0f);
		sizebox_dual->SetRenderOpacity(1.0f);
	}
	else
	{
		sizebox_one_People->SetRenderOpacity(1.0f);
		sizebox_one_Fireman->SetRenderOpacity(0.0f);
		sizebox_dual->SetRenderOpacity(0.0f);
		
	}
}

void UCinematicUI::NextWidgetStart()
{
	sizebox_one_People->SetRenderOpacity(0.0f);
	sizebox_one_Fireman->SetRenderOpacity(1.0f);
	sizebox_dual->SetRenderOpacity(0.0f);
}

void UCinematicUI::CloseWidget()
{
	RemoveFromParent();
}

void UCinematicUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 바인딩
	if (Skipbtn) Skipbtn->OnClicked.AddDynamic(this, &UCinematicUI::OnSkipClicked);
}

void UCinematicUI::OnSkipClicked()
{
	// 스킵 버튼 클릭
	UE_LOG(LogTemp, Warning, TEXT("skip btn"));
}
