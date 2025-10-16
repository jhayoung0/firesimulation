// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
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

	
};
