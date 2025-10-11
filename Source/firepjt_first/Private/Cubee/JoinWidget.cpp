// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/JoinWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"

void UJoinWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EdtTxt_Url)
	{
		EdtTxt_Url->OnTextCommitted.AddDynamic(this, &UJoinWidget::JoinURL);
	}
	
	if (Btn_Back)
	{
		Btn_Back->OnPressed.AddDynamic(this, &UJoinWidget::OnBackPressed);
	}
}

void UJoinWidget::OnBackPressed()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UJoinWidget::JoinURL(const FText& inText, ETextCommit::Type inCommitMethod)
{
	// Enter 키로만 접속 (실수 방지)
	if (inCommitMethod != ETextCommit::Type::OnEnter) return;

	FString IP = inText.ToString().TrimStartAndEnd();
	if (IP.IsEmpty())
	{
		IP = TEXT("127.0.0.1");
	}

	// 포트 번호가 없으면 기본 포트(7777) 추가
	// 서버가 로비 열어놓고 7777에서 대기 중임. 명시 안하면 127.0.0.1 안에서 헤맨다
	if (!IP.Contains(TEXT(":")))
	{
		IP = FString::Printf(TEXT("%s:7777"), *IP);
	}

	UE_LOG(LogTemp, Warning, TEXT("Joining server: %s"), *IP);
	
	// 서버 접속
	//UGameplayStatics::OpenLevel(this, *inText.ToString(), true);
	UGameplayStatics::OpenLevel(this, *IP, true);
}
