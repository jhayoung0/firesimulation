// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataAsset.h"
#include "InfoWidget.generated.h"

/**
 * 개별 텍스트 항목 구조체
 */
USTRUCT(BlueprintType)
struct FInfoTextEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
};

/*
 * 캐릭터별로 생성할 DataAsset 클래스
 */
UCLASS(BlueprintType)
class FIREPJT_FIRST_API UInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	TArray<FInfoTextEntry> InfoTexts;
};

UCLASS()
class FIREPJT_FIRST_API UInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
protected:
	// 사용할 데이터 에셋
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
	UInfoDataAsset* InfoDataAsset;
	
public:	
	// 제목
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Title;
	// 내용
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Description;
	// 등장 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;

public:
	// 각 캐릭터별 data asset 연결
	UFUNCTION(BlueprintCallable, Category = Info)
	void SetInfoDataAsset(UInfoDataAsset* NewDataAsset);

	// UI 띄우기. Idx에 data asset에 작성한 내용 중 원하는 인덱스 입력
	UFUNCTION(BlueprintCallable, Category = Info)
	void ViewInfoWidget(int32 Idx);

	// UI 지우기
	UFUNCTION(BlueprintCallable, Category = Info)
	void RemoveInfoWidget();
	
protected:
	// Data Asset에서 원하는 텍스트 가져오기
	void SetInfoTextFromIndex(int32 Index);

	// 텍스트박스 초기화
	void InitializeTextBlocks();
};
