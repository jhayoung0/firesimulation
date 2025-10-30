// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"



class UInfoDataAsset;
class UTextureRenderTarget2D;

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// 정보선 ui
	UPROPERTY(meta=(BindWidget))
	class USizeBox* SizeBox_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInfoDataAsset* peopleda;
	
	
	// SizeBox_Info 블루프린트 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInfoWidget> InfoWidget;
	// INFO UI 추가 함수
	void AddInfoUI(int32 idx);

	
	// 산소 바
	UPROPERTY(meta=(BindWidget))
	class UImage* Outline_Img;
	
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* OxygenBar;

	// 숨차는 느낌 UI 
	UPROPERTY(meta=(BindWidget))
	class UImage* DamageUI;

	UFUNCTION()
	void ShowDamageUI(float opacity);
	
	// 산소마스크 UI
	UPROPERTY(meta=(BindWidget))
	class UImage* OxygenMaskUI;
	
	float maskUIopacity;

	UFUNCTION()
	void ShowMaskUI(bool check);
	
	// 산소 바 산소 설정
	UFUNCTION()
	void SetOxygenPercent(float percent);
	
	// SubMission UI
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UOverlay> OverlaySubMission;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTextBlock> TextSubMission;

	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	TObjectPtr<class UTextBlock> TextContent;

	// SubMission Variable
	int32 CurrentSubMission = 1;

	// 실제 미션 수행 함수
	void HandleMission(int32 MissionIndex);
		
	// Submission 넘기기
	UFUNCTION()
	void NextSubMission();
	
	// Submission 넘기기
	UFUNCTION()
	void SuccessSubMission();
	
	// Submission 1
	UFUNCTION()
	void ShowOneSubMission(); // 서브미션 : 신고 전 주위 둘러보기

	// Submission 2
	UFUNCTION()
	void ShowTwoSubMission(); // 서브미션 : 신고하기 - 위치 , 사람 등.
	
	// Submission 3
	UFUNCTION()
	void ShowThreeSubMission(); // 서브미션 : 사람 안전한 곳으로 대피시키기

	// Submission 4
	UFUNCTION()
	void ShowFourSubMission(); // 서브미션 : 수건으로 코 막기
	
	// Submission 5
	UFUNCTION()
	void ShowFiveSubMission(); // 서브미션 : 마스크 착용하기

	// Submission 6
	UFUNCTION()
	void ShowSixSubMission(); // 서브미션 : 소방관 따라가기


	

	
};
