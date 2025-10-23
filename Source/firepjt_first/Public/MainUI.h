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


	/*
	UPROPERTY(meta=(BindWidget))
	UUserWidget* cinematicWBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	TObjectPtr<UTextureRenderTarget2D> CinematicRenderTarget;

	
	UFUNCTION()
	void ToggleCinematic(bool Open);
	*/

	

	
};
