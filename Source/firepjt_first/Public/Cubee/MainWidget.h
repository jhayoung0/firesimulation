// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Host;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Join;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	UPROPERTY(meta = (BindWidget))
	class UJoinWidget* JoinWidget;

	UFUNCTION()
	void OnHostClicked();
	UFUNCTION()
	void OnJoinClicked();
	
protected:
	// Server
	FString url;
	
	void CreateHost();
};
