// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CinematicUI.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UCinematicUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UImage* RT_People;

	UPROPERTY(meta=(BindWidget))
	class UImage* RT_Fireman;

	UPROPERTY(meta=(BindWidget))
	class UImage* RT_All_People;

	UPROPERTY(meta=(BindWidget))
	class UImage* RT_All_Fireman;
	
	UPROPERTY(meta=(BindWidget))
	class USizeBox* sizebox_dual;

	UPROPERTY(meta=(BindWidget))
	class USizeBox* sizebox_one_People;

	UPROPERTY(meta=(BindWidget))
	class USizeBox* sizebox_one_Fireman;


	UPROPERTY(meta=(BindWidget))
	class UTextBlock* completetext;

	
	UFUNCTION()
	void ChangeCompleteText(FString text);
	// 스킵 버튼 
	UPROPERTY(meta=(BindWidget))
	class UButton* Skipbtn;

	// 스킵버튼 없애기
	UFUNCTION()
	void HideSkipbtn();
	
	//토글
	UFUNCTION()
	void OpenWidgetToggle(int32 idx);  // 1이면 dual, 2면 people 3이면 fireman
	
	
	UFUNCTION()
	void CloseWidget();

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSkipClicked();
};
