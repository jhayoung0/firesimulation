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
	virtual void NativeDestruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Dialogue;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Next;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Select1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Select1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Select2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Select2;

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnSelectOneClicked();
	
	UFUNCTION()
	void OnSelectOTwoClicked();

public:
	// Dialogue
	UPROPERTY()
	class ANPCBase* OwningNPC;

	void SetDialogueText(int32 Index);
	void SetSelectionText(int32 Index);
};
