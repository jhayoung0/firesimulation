// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UJoinWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EdtTxt_Url;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;

	UFUNCTION()
	void OnBackPressed();

protected:
	UFUNCTION()
	void JoinURL(const FText& inText, ETextCommit::Type inCommitMethod);
};
