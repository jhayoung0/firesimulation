// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "firepjt_firstPlayerController.h"
#include "PhoneWidget.h"
#include "PeopleOnePC.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API APeopleOnePC : public Afirepjt_firstPlayerController
{
	GENERATED_BODY()

public: // widget
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void OpenPhoneUI();

	UFUNCTION(BlueprintCallable)
	void ClosePhoneUI();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UPhoneWidget> phonewidget;

	UPROPERTY(Transient)
	UPhoneWidget* phoneUI = nullptr;

	
};
