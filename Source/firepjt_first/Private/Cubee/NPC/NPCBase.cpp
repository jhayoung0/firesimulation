// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/NPC/NPCBase.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Cubee/NPC/NPCWidget.h"

// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentDialogueID = -1;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPCBase::StartDialogue()
{
	if (!DialogueTable) return;

	CurrentDialogueID = 0;
	IsTalking = true;

	// 위젯 띄우기
	if (NPCWidgetClass)
	{
		NPCWidget = CreateWidget<UNPCWidget>(GetWorld(), NPCWidgetClass);
		if (NPCWidget)
		{
			NPCWidget->OwningNPC = this;

			NPCWidget->AddToViewport();
			NPCWidget->SetDialogueText(0);
		}
	}
}

void ANPCBase::Multicast_StartDialogue_Implementation()
{
	StartDialogue();
}

FDialogueEntry* ANPCBase::GetDialogueByID(int32 DialogueID)
{
	if (!DialogueTable) return nullptr;

	FString ContextString = FString::Printf(TEXT("Get Dialogue ID: %d"), DialogueID);
	FDialogueEntry* DialogueEntry = DialogueTable->FindRow<FDialogueEntry>(FName(*FString::FromInt(DialogueID)), ContextString);

	return DialogueEntry;
}

void ANPCBase::ProgressDialogue(int32 NextID)
{
	// 대화 종료
	if(NextID < 0)
	{
		EndDialogue();
		return;
	}

	NPCWidget->Btn_Select1->SetVisibility(ESlateVisibility::Hidden);
	NPCWidget->Btn_Select2->SetVisibility(ESlateVisibility::Hidden);
	NPCWidget->Btn_Next->SetVisibility(ESlateVisibility::Visible);

	CurrentDialogueID = NextID;
	
	NPCWidget->SetDialogueText(CurrentDialogueID);
	
	FDialogueEntry* CurrentDialogue = GetDialogueByID(CurrentDialogueID);
	if (CurrentDialogue)
	{
		// 애님 몽타주 실행
		if (CurrentDialogue->DialogueAnimation)
		{
			GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
			PlayAnimMontage(CurrentDialogue->DialogueAnimation);
		}

		// 선택지 생성
		if (CurrentDialogue->PlayerChoices.Num() > 0)
		{
			NPCWidget->Btn_Select1->SetVisibility(ESlateVisibility::Visible);
			NPCWidget->Btn_Select2->SetVisibility(ESlateVisibility::Visible);
			NPCWidget->Btn_Next->SetVisibility(ESlateVisibility::Hidden);

			NPCWidget->SetSelectionText(CurrentDialogueID);
		}
	}
}

void ANPCBase::Multicast_ProgressDialogue_Implementation(int32 NextID)
{
	ProgressDialogue(NextID);
}

void ANPCBase::EndDialogue()
{
	CurrentDialogueID = -1;
	IsTalking = false;
	
	NPCWidget->RemoveFromParent();
	
}


