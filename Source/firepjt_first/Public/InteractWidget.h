// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* interactKey;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* description;

	// 텍스트 변경 함수
	UFUNCTION()
	void SetInteractText(FString key, FString desc);
};
