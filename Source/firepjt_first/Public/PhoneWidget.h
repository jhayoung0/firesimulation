// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PhoneWidget.generated.h"

/**
 * 
 */

// 델리게이트 선언 (번호 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialCall, const FString&, DialNumber);

UCLASS()
class FIREPJT_FIRST_API UPhoneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Phone")
	FOnDialCall OnDialCall;

	
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* callText;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* callButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= sfx)
	class USoundBase* callSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= sfx)
    class USoundBase* callbuttonSound;
		
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

	virtual void NativeConstruct() override;



};
