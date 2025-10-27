// Fill out your copyright notice in the Description page of Project Settings.


#include "FiremanMainUI.h"
#include "Components/SizeBox.h"
#include "Cubee/InfoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FireActor.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"


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

	ConstructorHelpers::FClassFinder<AFireActor> fireActorRef(TEXT("/Game/CustomContents/Fireman/Fire/BP_Fire.BP_Fire_C"));
	if (fireActorRef.Succeeded())
	{
		FireActorClass = fireActorRef.Class;
	}
}

void UFiremanMainUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<AActor*> fireActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FireActorClass, fireActors);
	FireMaxNum = fireActors.Num();

	// fireactor delegate
	for (auto& fireActor : fireActors)
	{
		fireActor->OnDestroyed.AddDynamic(this, &UFiremanMainUI::UpdateFireCurrentNum);
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

void UFiremanMainUI::ShowSubMissionUI(ESubMissions subMission)
{
	switch (subMission)
	{
	case ESubMissions::FireOff:
		ShowFireOffSubMission();
		break;
	case ESubMissions::MaskOut:
		ShowMaskOutSubMission();
		break;
	case ESubMissions::ForceOpenDoor:
		ShowForceOpenDoorSubMission();
		break;
	}
}

void UFiremanMainUI::SuccessSubMission()
{
	// 통과! 보여주고 1초 뒤 다음 서브 미션 시작
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		TextContent->SetText(FText::FromString(TEXT("통과!")));
		CurrentSubMission++;

		// Out of Range in ESubMissions
		if (CurrentSubMission == 3)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				OverlaySubMission->SetVisibility(ESlateVisibility::Hidden);
			}, 1, false);
			return;
		}
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			ShowSubMissionUI(static_cast<ESubMissions>(CurrentSubMission));
		}, 1, false);
		
	}, 0.5f, false);
}

void UFiremanMainUI::ShowFireOffSubMission()
{
	OverlaySubMission->SetVisibility(ESlateVisibility::Visible);
	TextSubMission->SetText(FText::FromString(TEXT("1층 화재 진압")));

	FString str = FString::Printf(TEXT("남은 불 %d / %d"), FireCurrentNum, FireMaxNum);
	TextContent->SetText(FText::FromString(str));
}

void UFiremanMainUI::UpdateFireCurrentNum(AActor* DestroyedActor)
{
	FString str = FString::Printf(TEXT("남은 불 %d / %d"), ++FireCurrentNum, FireMaxNum);
	TextContent->SetText(FText::FromString(str));

	if (FireCurrentNum == FireMaxNum)
	{
		SuccessSubMission();
	}
}

void UFiremanMainUI::ShowMaskOutSubMission()
{
	TextSubMission->SetText(FText::FromString(TEXT("산소 마스크 지급")));
	TextContent->SetText(FText::FromString(TEXT("요구조자에게 가까이 다가가\nR키 누르기")));

	
}

void UFiremanMainUI::ShowForceOpenDoorSubMission()
{
	TextSubMission->SetText(FText::FromString(TEXT("옥상 문 강제 개방")));
	TextContent->SetText(FText::FromString(TEXT("쇠지렛대를 들어 문을 여세요.")));
}
