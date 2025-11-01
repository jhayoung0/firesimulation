// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/LobbyWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Cubee/LobbyGameMode.h"
#include "firepjt_firstPlayerController.h"
#include "Components/Image.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Btn_Start)
	{
		Btn_Start->OnClicked.AddDynamic(this, &ULobbyWidget::OnStartClicked);
	}

	if (Btn_Firefighter)
	{
		Btn_Firefighter->OnClicked.AddDynamic(this, &ULobbyWidget::OnFirefighterClicked);
	}

	if (Btn_Citizen)
	{
		Btn_Citizen->OnClicked.AddDynamic(this, &ULobbyWidget::OnCitizenClicked);
	}
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->HasAuthority())
		{
			ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
			GM->OnPlayersEnough.AddDynamic(this, &ULobbyWidget::ActivateStartButton);

			Btn_Start->SetIsEnabled(false);
			TextStart->SetText(FText::FromString(TEXT("대기중...")));
			
		}
		else
		{
			Btn_Start->SetIsEnabled(false);
			TextStart->SetText(FText::FromString(TEXT("시작 대기")));
		}
	}
	
}

// 체크 이미지 활성화
void ULobbyWidget::ShowCheckImg(bool isFireman)
{

	
	
	// 로컬 플레이어가 아니면 함수 나가자
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (!PC->IsLocalController())
		{
			return; // not our screen, skip visuals
		}
	}

	
	if (isFireman)
	{
		// 체크 이미지 활성화
		img_check_fire->SetRenderOpacity(1.0f);
		img_check_citizen->SetRenderOpacity(0.0f);
		// 이미지 색상 변경 
		image_1->SetBrushTintColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		image_2->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		img_check_fire->SetRenderOpacity(0.0f);
		img_check_citizen->SetRenderOpacity(1.0f);
		// 이미지 색상 변경 
		image_2->SetBrushTintColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		image_1->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	
}

void ULobbyWidget::OnStartClicked()
{
	//if (!bIsHost) return;

	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->StartGame();
	}
}

void ULobbyWidget::OnFirefighterClicked()
{
	// 체크 이미지 활성화
	ShowCheckImg(true);
	
	// PlayerController를 통해 Server RPC 호출
	if (APlayerController* PC = GetOwningPlayer())
	{
		Afirepjt_firstPlayerController* CustomPC = Cast<Afirepjt_firstPlayerController>(PC);
		if (CustomPC)
		{
		
			CustomPC->Server_SetPlayerRole(EPlayerRole::Firefighter);
			UE_LOG(LogTemp, Warning, TEXT("[LobbyWidget] Firefighter role selected by %s"), *PC->GetName());
		}
	}
}

void ULobbyWidget::OnCitizenClicked()
{

	// 체크 이미지 활성화
	ShowCheckImg(false);
	
	// PlayerController를 통해 Server RPC 호출
	if (APlayerController* PC = GetOwningPlayer())
	{
		Afirepjt_firstPlayerController* CustomPC = Cast<Afirepjt_firstPlayerController>(PC);
		if (CustomPC)
		{
	
			CustomPC->Server_SetPlayerRole(EPlayerRole::Citizen);
			UE_LOG(LogTemp, Warning, TEXT("[LobbyWidget] Citizen role selected by %s"), *PC->GetName());
		}
	}
}

void ULobbyWidget::UpdatePlayerCount(int32 Current, int32 Max)
{
	if (Txt_PlayerCount)
	{
		Txt_PlayerCount->SetText(FText::FromString(FString::Printf(TEXT("Players : %d / %d"), Current, Max)));
	}
}

void ULobbyWidget::ActivateStartButton()
{
	Btn_Start->SetIsEnabled(true);
	TextStart->SetText(FText::FromString(TEXT("훈련 시작")));
}


