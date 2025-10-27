// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/VictoryWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UVictoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Back)
	{
		Btn_Back->OnClicked.AddDynamic(this, &UVictoryWidget::OnBackButtonClicked);
	}
}

void UVictoryWidget::OnBackButtonClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	// 서버와 클라이언트 구분하여 처리
	if (PC->HasAuthority() && PC->IsLocalController())
	{
		// Listen Server (호스트) - OpenLevel로 이동
		UE_LOG(LogTemp, Warning, TEXT("[GameOverWidget] Server returning to main menu"));
		UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuMap"));
	}
	else if (!PC->HasAuthority())
	{
		// 클라이언트 - 세션 연결 끊고 메인 메뉴로
		UE_LOG(LogTemp, Warning, TEXT("[GameOverWidget] Client returning to main menu"));
		PC->ClientTravel("/Game/CustomContents/House/MainMenuMap", ETravelType::TRAVEL_Absolute);
	}
}
