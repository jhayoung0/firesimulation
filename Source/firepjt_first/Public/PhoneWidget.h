// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "PhoneWidget.generated.h"

/**
 * 
 */

// 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequestPlayCinematic); 

UCLASS()
class FIREPJT_FIRST_API UPhoneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintAssignable, Category="Cinematic")
	FOnRequestPlayCinematic OnRequestPlayCinematic;


public:
	/*
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* callText;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* callButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= sfx)
	class USoundBase* callSound;
	

		
	// 버튼 1~9
	UPROPERTY(meta = (BindWidget)) UButton* Btn_1;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_2;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_3;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_4;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_5;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_6;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_7;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_8;
	UPROPERTY(meta = (BindWidget)) UButton* Btn_9;

	UFUNCTION()
	void ResetTEXT();
	
	UFUNCTION()
	void AppendDigit(const FString& Digit);

	UFUNCTION(BlueprintCallable)
	void TryCall();
	
	
	UPROPERTY(EditDefaultsOnly, Category="Dial")
	int32 MaxLen = 11;

	FString RawNumber;
	
	
	// 버튼 클릭 콜백
	UFUNCTION()
	void OnNumberClicked_1();
	UFUNCTION()
	void OnNumberClicked_2();
	UFUNCTION()
	void OnNumberClicked_3();
	UFUNCTION()
	void OnNumberClicked_4();
	UFUNCTION()
	void OnNumberClicked_5();
	UFUNCTION()
	void OnNumberClicked_6();
	UFUNCTION()
	void OnNumberClicked_7();
	UFUNCTION()
	void OnNumberClicked_8();
	UFUNCTION()
	void OnNumberClicked_9();

	// 오버레이 변경하기
	UPROPERTY(meta = (BindWidget))
	UOverlay* Overlay_Call;
	*/

	UPROPERTY(meta = (BindWidget))
	UOverlay* Overlay_Message;

	/*
	// 오버레이 전환 함수
	UFUNCTION()
	void UpdateOverlayVisibility(bool bShown);
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= sfx)
	class USoundBase* callbuttonSound;

	
	// 채팅 기능 추가
	// 채팅 내용 담고 있는 Scroll box
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* scrollChat;
	// 채팅 내용 입력하는 Editable Textbox
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* editChat;

	// 채팅 입력 후 엔터 쳤을 때 호출되는 함수
	UFUNCTION()
	void OnTextBoxCommit(const FText& text, ETextCommit::Type commitMethod);

	//chatwidget 블루프린트 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UChatWidget> chatWidget;

	// 채팅 UI 추가 함수
	void AddChat(FString text);

	// 미션 완료 검사 함수
	UFUNCTION()
	void CheckMission(FString text);

	// 미션 초기화 함수
	UFUNCTION()
	void InitMission();

	bool bContainAddress = false;
	bool bContainsFireSituation = false;
	bool bContainsPeopleSituation = false;
	
	
};
