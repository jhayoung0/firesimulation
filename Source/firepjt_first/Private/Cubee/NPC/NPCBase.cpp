// Fill out your copyright notice in the Description page of Project Settings.


#include "Cubee/NPC/NPCBase.h"

#include "Blueprint/UserWidget.h"
#include "Cubee/NPC/NPCWidget.h"

// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentDialogueID = -1;
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

	// 서버, 클라 양쪽에 어떻게 띄우는지는 스킵 버튼 추적할 것 (아마 RPC썼을 거 같긴 한데)
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

	CurrentDialogueID = NextID;

	NPCWidget->SetDialogueText(CurrentDialogueID);
}

void ANPCBase::EndDialogue()
{
	CurrentDialogueID = -1;
	NPCWidget->RemoveFromParent();
	
	UE_LOG(LogTemp, Log, TEXT("Dialogue Ended"));
}


