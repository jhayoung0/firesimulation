// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/NPC/NPCWidget.h"

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

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
	}
}

void UNPCWidget::OnNextClicked()
{
	if (OwningNPC)
	{
		FDialogueEntry* CurrentDialogue = OwningNPC->GetDialogueByID(OwningNPC->GetCurrentDialogueID());
		if (CurrentDialogue)
		{
			OwningNPC->ProgressDialogue(CurrentDialogue->NextDialogueID);
			UE_LOG(LogTemp, Warning, TEXT("Button pressed"));
		}
	}
}

void UNPCWidget::SetDialogueText(int32 Index)
{
	UE_LOG(LogTemp, Display, TEXT("SetDialogueText"));
	
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


