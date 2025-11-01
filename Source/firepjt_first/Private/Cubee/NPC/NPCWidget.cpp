// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/NPC/NPCWidget.h"

#include "firepjt_firstPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Cubee/NPC/NPCBase.h"

void UNPCWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Next)
	{
		Btn_Next->OnClicked.AddDynamic(this, &UNPCWidget::OnNextClicked);
	}

	if (Btn_Select1)
	{
		Btn_Select1->OnClicked.AddDynamic(this, &UNPCWidget::OnSelectOneClicked);
		Btn_Select1->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (Btn_Select2)
	{
		Btn_Select2->OnClicked.AddDynamic(this, &UNPCWidget::OnSelectOTwoClicked);
		Btn_Select2->SetVisibility(ESlateVisibility::Hidden);
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
	}
}

void UNPCWidget::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void UNPCWidget::OnNextClicked()
{
	Afirepjt_firstPlayerController* PC = Cast<Afirepjt_firstPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC && OwningNPC)
	{
		FDialogueEntry* CurrentDialogue = OwningNPC->GetDialogueByID(OwningNPC->GetCurrentDialogueID());
		if (CurrentDialogue)
		{
			PC->Server_ProgressDialogue(CurrentDialogue->NextDialogueID);
		}
	}
	
	/*
	if (OwningNPC)
	{
		FDialogueEntry* CurrentDialogue = OwningNPC->GetDialogueByID(OwningNPC->GetCurrentDialogueID());
		if (CurrentDialogue)
		{
			OwningNPC->ProgressDialogue(CurrentDialogue->NextDialogueID);
		}
	}
	*/
}

void UNPCWidget::OnSelectOneClicked()
{
	Afirepjt_firstPlayerController* PC = Cast<Afirepjt_firstPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC && OwningNPC)
	{
		PC->Server_ProgressDialogue(OwningNPC->GetCurrentDialogueID()+1);
	}
}

void UNPCWidget::OnSelectOTwoClicked()
{
	Afirepjt_firstPlayerController* PC = Cast<Afirepjt_firstPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC && OwningNPC)
	{
		PC->Server_ProgressDialogue(OwningNPC->GetCurrentDialogueID()+2);
	}
}

void UNPCWidget::SetDialogueText(int32 Index)
{
	if (OwningNPC)
	{
		FDialogueEntry* Dialogue = OwningNPC->GetDialogueByID(Index);
		if (Dialogue != nullptr)
		{
			if (Txt_Name)
			{
				Txt_Name->SetText(Dialogue->SpeakerName);
			}

			if (Txt_Dialogue)
			{
				Txt_Dialogue->SetText(Dialogue->DialogueText);
			}
		}
	}
}

void UNPCWidget::SetSelectionText(int32 Index)
{
	if (OwningNPC)
	{
		FDialogueEntry* Dialogue = OwningNPC->GetDialogueByID(Index);
		if (Dialogue != nullptr)
		{
			if (Txt_Select1)
			{
				Txt_Select1->SetText(Dialogue->PlayerChoices[0]);
			}

			if (Txt_Select2)
			{
				Txt_Select2->SetText(Dialogue->PlayerChoices[1]);
			}
		}
	}
}


