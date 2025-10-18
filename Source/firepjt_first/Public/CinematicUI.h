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
	
	//토글
	UFUNCTION()
	void OpenWidgetToggle(bool isdual);

	UFUNCTION()
	void NextWidgetStart();
	
	UFUNCTION()
	void CloseWidget();
	
};
