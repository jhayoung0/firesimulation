// Fill out your copyright notice in the Description page of Project Settings.


#include "PhoneWidget.h"

#include "ASequencePlayer.h"
#include "NiagaraValidationRule.h"
#include "PeopleBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Internationalization/Regex.h"
#include "Components/TextBlock.h"
#include "Cubee/InGameWidget.h"
#include "Kismet/GameplayStatics.h"



void UPhoneWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (editChat)                                                                                                                                                     
	{                                                                                                                                                                 
		editChat->OnTextCommitted.RemoveDynamic(this, &UPhoneWidget::OnTextBoxCommit);                                                                                
		editChat->OnTextCommitted.AddDynamic(this, &UPhoneWidget::OnTextBoxCommit);                                                                                   
	}   
	/*
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
	*/
}


// 엔터쳤을 때 호출
void UPhoneWidget::OnTextBoxCommit(const FText& text,
	ETextCommit::Type commitMethod)
{
	// 만약에 엔터를 친 이벤트라면 
	if (commitMethod == ETextCommit::OnEnter)
	{
		// 버튼 sfx
		UGameplayStatics::PlaySound2D(this, callbuttonSound);
		
		// 채팅 UI 추가
		AddChat(text.ToString());
		// enter 친 텍스트에 대해 미션 완료 여부 검사
		CheckMission(text.ToString());
	}
	else if (commitMethod == ETextCommit::OnCleared)
	{
		// 강제로 editchat을 활성화
		editChat->SetFocus();
	}
}

void UPhoneWidget::AddChat(FString text)
{
	// 현재 스크롤 위치값
	float scrollOffset = scrollChat->GetScrollOffset();
	// 스크롤이 맨 끝일 때 값
	float scrollOffsetOfEnd = scrollChat->GetScrollOffsetOfEnd();
	
	// 채팅 ui만들어서 채팅 내용에 추가
	UChatWidget* chat = CreateWidget<UChatWidget>(GetWorld(), chatWidget);
	scrollChat->AddChild(chat);
	chat->SetContent(text);
		
	// editchat에 남아있는 내용 초기화
	editChat->SetText(FText());

	// 만약에 스크롤이 위치가 맨 끝이라면
	if (scrollOffset == scrollOffsetOfEnd)
	{
		// 개행되는 채팅이 추가되면 한줄로 크기를 인식해서 발생하는 문제 때문에
		// scrollto end를 0.01초 뒤에 실행
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			// 스크롤 위치를 맨 끝으로 해라!
			scrollChat->ScrollToEnd();
		}, 0.01f, false);
	}
}

void UPhoneWidget::CheckMission(FString text)
{
	auto* peoplebase = Cast<APeopleBase>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	
	// 미션 완료 여부 검사 
	if (text.Contains(TEXT("205")) && text.Contains(TEXT("아파트")))
	{
		bContainAddress = true;
		peoplebase->mainui->ChangePhoneSubMission(1);
	}
	else if (FRegexMatcher(
			FRegexPattern(TEXT("부상자.*?(한\\s*명|1\\s*명)|(한\\s*명|1\\s*명).*?부상자")), 
			text
		).FindNext())

	{
		bContainsPeopleSituation = true;
		peoplebase->mainui->ChangePhoneSubMission(2);
	}
	else if (text.Contains(TEXT("복도")) && text.Contains(TEXT("불")))
	{
		bContainsFireSituation = true;
		peoplebase->mainui->ChangePhoneSubMission(3);
	}

	if (bContainAddress + bContainsPeopleSituation + bContainsFireSituation == 3)
	{
		
		// 서브미션 안넘겼으면 미션 안넘어감
		if (peoplebase->mainui->CurrentSubMission == 2)
		{
			peoplebase->Interaction();
			// 다음 미션으로 넘기기
			peoplebase->GoNextMission();
			// 다음 서브미션으로 넘기기
			peoplebase->mainui->SuccessSubMission();
		}
		else if (peoplebase->mainui->CurrentSubMission < 2)
		{
			peoplebase->mainui->SubMissionWarning();
		}

	}
}

void UPhoneWidget::InitMission()
{
	// 전부 초기화
	bContainAddress = false;
	bContainsFireSituation = false;
	bContainsPeopleSituation = false;
}

/*
void UPhoneWidget::ResetTEXT()
{
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
void UPhoneWidget::OnNumberClicked_1() { UE_LOG(LogTemp,Warning, TEXT("1눌림")); AppendDigit(TEXT("1")); }
void UPhoneWidget::OnNumberClicked_2() { AppendDigit(TEXT("2")); }
void UPhoneWidget::OnNumberClicked_3() { AppendDigit(TEXT("3")); }
void UPhoneWidget::OnNumberClicked_4() { AppendDigit(TEXT("4")); }
void UPhoneWidget::OnNumberClicked_5() { AppendDigit(TEXT("5")); }
void UPhoneWidget::OnNumberClicked_6() { AppendDigit(TEXT("6")); }
void UPhoneWidget::OnNumberClicked_7() { AppendDigit(TEXT("7")); }
void UPhoneWidget::OnNumberClicked_8() { AppendDigit(TEXT("8")); }
void UPhoneWidget::OnNumberClicked_9() { AppendDigit(TEXT("9")); }


// 전화 화면 -> 메시지 화면 전환
void UPhoneWidget::UpdateOverlayVisibility(bool MessageShow)
{
	if (MessageShow)
	{
		UE_LOG(LogTemp, Warning, TEXT("작동"));
		Overlay_Message->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Overlay_Call->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Overlay_Message->SetVisibility(ESlateVisibility::Hidden);
		Overlay_Call->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}


void UPhoneWidget::TryCall()
{
	
	if (RawNumber== TEXT("119"))
	{
	
		auto* peoplebase = Cast<APeopleBase>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
		// 전화 sfx
		UGameplayStatics::PlaySound2D(this, callSound);

		// 메시지 화면으로 넘기기
		UpdateOverlayVisibility(true);
		
	}
	else
	{
		return;
	}
}

*/