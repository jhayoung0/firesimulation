// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Cubee/InfoWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"


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


void UMainUI::UpdateOverlayVisibility(bool bShown)
{
	// true면 핸드폰 미션 메시지 나오기
	if (bShown)
	{
		OverlaySubMission_1->SetVisibility(ESlateVisibility::Hidden);
		OverlaySubMission_2->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OverlaySubMission_1->SetVisibility(ESlateVisibility::Visible);
		OverlaySubMission_2->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 서브미션~~~&&
void UMainUI::HandleMission(int32 MissionIndex)
{
	switch (MissionIndex)
	{
	case 1:
		ShowOneSubMission();
		break;
	case 2:
		ShowTwoSubMission();
		break;
	case 3:
		ShowThreeSubMission();
		break;
	case 4:
		ShowFourSubMission();
		break;
	case 5:
		ShowFiveSubMission();
		break;
	case 6:
		ShowSixSubMission();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("All missions completed."));
		break;
	}
}

void UMainUI::NextSubMission()
{
	CurrentSubMission++;
	UE_LOG(LogTemp, Warning, TEXT("Now entering Mission %d"), CurrentSubMission);
	HandleMission(CurrentSubMission);
}


// 성공 함수 호출
void UMainUI::SuccessSubMission()
{
	// 통과! 보여주고
	TextContent->SetText(FText::FromString(TEXT("통과!")));
	// 2초 뒤 다음 서브 미션 시작
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMainUI::NextSubMission, 1.0f, false);
}

void UMainUI::SubMissionWarning()
{
	// 텍스트 알럿 보여주기 3초 뒤에 닫기
	TextAlert->SetVisibility(ESlateVisibility::Visible);
	TextAlert->SetText(FText::FromString(TEXT("미션을 순서대로 수행해주세요.")));

	FTimerHandle timerhandle;
	GetWorld()->GetTimerManager().SetTimer(timerhandle, [&]()
	{
		TextAlert->SetVisibility(ESlateVisibility::Hidden);
	}, 3, false);
	
}


void UMainUI::ShowOneSubMission()
{
	OverlaySubMission_1->SetVisibility(ESlateVisibility::Visible);
	TextSubMission->SetText(FText::FromString(TEXT("신고 전 주변 상황 확인")));
	TextContent->SetText(FText::FromString(TEXT("대피가 가능한지 확인하고,\n요구조자 인원 파악하기")));
}


void UMainUI::ShowTwoSubMission()
{
	// 핸드폰 미션 시작
	UpdateOverlayVisibility(true);
	TextSubMission_1->SetText(FText::FromString(TEXT("휴대폰을 찾아 신고")));
	TextContent_1->SetText(FText::FromString(TEXT("- 위치 신고 (건물명, 호수)")));
	TextContent_2->SetText(FText::FromString(TEXT("- 부상자 정보 전달")));
	TextContent_3->SetText(FText::FromString(TEXT("- 화재 상황 설명")));	
}

void UMainUI::ChangePhoneSubMission(int32 idx)
{
	if (idx == 1)
	{
		TextContent_1->SetColorAndOpacity(FLinearColor::Gray);
	}
	else if (idx == 2)
	{
		TextContent_2->SetColorAndOpacity(FLinearColor::Gray);
	}
	else if (idx == 3)
	{
		TextContent_3->SetColorAndOpacity(FLinearColor::Gray);
	}
}


void UMainUI::ShowThreeSubMission()
{
	// 핸드폰 미션 끝내고 다음 미션으로 넘어가기
	UpdateOverlayVisibility(false);
	TextSubMission->SetText(FText::FromString(TEXT("쓰러진 이웃 구조")));
	TextContent->SetText(FText::FromString(TEXT("안전한 곳으로\n이웃을 이동시키기")));	
}

void UMainUI::ShowFourSubMission()
{
	TextSubMission->SetText(FText::FromString(TEXT("물수건 찾기")));
	TextContent->SetText(FText::FromString(TEXT("유독 가스를 막을 수 있는\n도구 찾기")));	
}

void UMainUI::ShowFiveSubMission()
{
	TextSubMission->SetText(FText::FromString(TEXT("구조대 만나기")));
	TextContent->SetText(FText::FromString(TEXT("구조대를 만나, 마스크\n건네받기")));	
}

void UMainUI::ShowSixSubMission()
{
	TextSubMission->SetText(FText::FromString(TEXT("대피")));
	TextContent->SetText(FText::FromString(TEXT("소방관이 안내하는\n경로로 따라가기")));	
}

