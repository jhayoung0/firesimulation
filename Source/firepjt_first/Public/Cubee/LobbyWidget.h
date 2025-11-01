// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_PlayerCount;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Firefighter;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Citizen;

	// 안내 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextHead;

	// 스타트 버튼 텍스트
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextStart;
	
	// 체크 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* img_check_fire;

	UPROPERTY(meta = (BindWidget))
	class UImage* img_check_citizen;

	// 역할 이미지
	// 소방관
	UPROPERTY(meta = (BindWidget))
	class UImage* image_1;

	// 시민
	UPROPERTY(meta = (BindWidget))
	class UImage* image_2;

	
	// 체크 이미지 활성화
	UFUNCTION()
	void ShowCheckImg(bool isFireman);
	
	// 역할 클릭 시 호출되는 함수
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnFirefighterClicked();
	
	UFUNCTION()
	void OnCitizenClicked();

public:
	// Player count
	void UpdatePlayerCount(int32 Current, int32 Max);

	// Btn_Start 활성화
	UFUNCTION()
	void ActivateStartButton();
};
