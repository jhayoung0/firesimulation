// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UNPCWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Dialogue;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Next;

	UFUNCTION()
	void OnNextClicked();

public:
	// Dialogue
	UPROPERTY()
	class ANPCBase* OwningNPC;

	void SetDialogueText(int32 Index);
};
