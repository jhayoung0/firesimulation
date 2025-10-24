// Fill out your copyright notice in the Description page of Project Settings.


#include "PhoneWidget.h"

#include "ASequencePlayer.h"
#include "PeopleBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPhoneWidget::ResetTEXT()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset Text call")); 
	RawNumber.Empty();
	if (callText)
	{
		callText->SetText(FText::GetEmpty());
	}
}

void UPhoneWidget::AppendDigit(const FString& Digit)
{
	// 버튼 sfx
	UGameplayStatics::PlaySound2D(this, callbuttonSound);
	// 숫자만 허용 
	if (Digit.Len() == 1 && FChar::IsDigit(Digit[0]))
	{
		if (RawNumber.Len() < MaxLen)
		{
			RawNumber += Digit;
			if (callText)
				callText->SetText(FText::FromString(RawNumber));
		}
	}
}



// 버튼 클릭 시 숫자 추가
void UPhoneWidget::OnNumberClicked_1() { AppendDigit(TEXT("1")); }
void UPhoneWidget::OnNumberClicked_2() { AppendDigit(TEXT("2")); }
void UPhoneWidget::OnNumberClicked_3() { AppendDigit(TEXT("3")); }
void UPhoneWidget::OnNumberClicked_4() { AppendDigit(TEXT("4")); }
void UPhoneWidget::OnNumberClicked_5() { AppendDigit(TEXT("5")); }
void UPhoneWidget::OnNumberClicked_6() { AppendDigit(TEXT("6")); }
void UPhoneWidget::OnNumberClicked_7() { AppendDigit(TEXT("7")); }
void UPhoneWidget::OnNumberClicked_8() { AppendDigit(TEXT("8")); }
void UPhoneWidget::OnNumberClicked_9() { AppendDigit(TEXT("9")); }


void UPhoneWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 바인딩
	if (Btn_1)
	{
		Btn_1->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_1);
		Btn_1->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_1);
	}
	if (Btn_2)
	{
		Btn_2->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_2);
		Btn_2->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_2);
	}
	if (Btn_3)
	{
		Btn_3->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_3);
		Btn_3->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_3);
	}
	if (Btn_4)
	{
		Btn_4->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_4);
		Btn_4->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_4);
	}
	if (Btn_5)
	{
		Btn_5->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_5);
		Btn_5->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_5);
	}
	if (Btn_6)
	{
		Btn_6->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_6);
		Btn_6->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_6);
	}
	if (Btn_7)
	{
		Btn_7->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_7);
		Btn_7->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_7);
	}
	if (Btn_8)
	{
		Btn_8->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_8);
		Btn_8->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_8);
	}
	if (Btn_9)
	{
		Btn_9->OnClicked.RemoveDynamic(this, &UPhoneWidget::OnNumberClicked_9);
		Btn_9->OnClicked.AddDynamic(this, &UPhoneWidget::OnNumberClicked_9);
	}
	if (callButton)
	{
		callButton->OnClicked.RemoveDynamic(this, &UPhoneWidget::TryCall);
		callButton->OnClicked.AddDynamic(this, &UPhoneWidget::TryCall);
	}
}



void UPhoneWidget::TryCall()
{
	
	if (RawNumber== TEXT("119"))
	{
		auto* world = GetWorld();
		auto* peoplebase = Cast<APeopleBase>(UGameplayStatics::GetPlayerPawn(world,0));
		peoplebase->Interaction();
		// 전화 sfx
		UGameplayStatics::PlaySound2D(this, callSound);

		// 델리게이트 브로드캐스트 (시네마틱 재생)
		OnRequestPlayCinematic.Broadcast();

		// 다음 미션으로 넘기기
		peoplebase->GoNextMission();
	}
	else
	{
		return;
	}
}
