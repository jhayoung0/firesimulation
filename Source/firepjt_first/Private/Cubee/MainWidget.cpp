// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/MainWidget.h"

#include "SocketSubsystem.h"
#include "Components/Button.h"
#include "Cubee/JoinWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Host)
	{
		Btn_Host->OnClicked.AddDynamic(this, &UMainWidget::OnHostClicked);
	}

	if (Btn_Join)
	{
		Btn_Join->OnClicked.AddDynamic(this, &UMainWidget::OnJoinClicked);
	}
}

void UMainWidget::OnHostClicked()
{
	CreateHost();
}

void UMainWidget::OnJoinClicked()
{
	JoinWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidget::CreateHost()
{
	// 로컬 네트워크 IP 가져오기
	bool bCanBindAll;
	TSharedPtr<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);

	// IP 저장 (게스트에게 알려줄 용도)
	url = LocalAddr.IsValid() ? LocalAddr->ToString(false) : "127.0.0.1";

	UE_LOG(LogTemp, Display, TEXT("Host URL : %s"), *url);

	// Listen Server로 게임 시작
	UGameplayStatics::OpenLevel(this, FName("LobbyMap"), true, "listen");
}


