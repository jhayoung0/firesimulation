// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FiremanMainUI.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UFiremanMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFiremanMainUI(const FObjectInitializer& ObjectInitializer);
	
public:
	// INFO UI 추가 함수
	void AddInfoUI(int32 idx);
	
protected:
	// Info UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInfoWidget> InfoWidgetClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class USizeBox> SizeBox_Info;

	// Info UI DataAsset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UInfoDataAsset> DAFireman;
};
