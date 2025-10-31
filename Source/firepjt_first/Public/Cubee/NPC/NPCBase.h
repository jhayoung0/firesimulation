// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

// 대화 데이터 구조체
USTRUCT(BlueprintType)
struct FDialogueEntry : public FTableRowBase
{
	GENERATED_BODY()

	// NPC 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpeakerName;
	// 대화 내용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogueText;
	// 플레이어 선택지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> PlayerChoices;
	// 다음 대화 ID (-1 : 대화 종료)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NextDialogueID = -1;
	// 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DialogueAnimation = nullptr;
};

UCLASS()
class FIREPJT_FIRST_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> NPCWidgetClass;

	UPROPERTY()
	class UNPCWidget* NPCWidget;
	
protected:
	// Dialogue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue)
	UDataTable* DialogueTable;

	// 현재 진행중인 ID
	UPROPERTY(BlueprintReadOnly, Category = Dialogue)
	int32 CurrentDialogueID;

public:
	FORCEINLINE int32 GetCurrentDialogueID() {return CurrentDialogueID;}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dialogue)
	bool IsTalking = false;
	
	// 대화 시작
	UFUNCTION(BlueprintCallable, Category = Dialogue)
	void StartDialogue();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartDialogue();
	
	// 데이터 테이블에서 특정 ID 데이터 가져오기
	FDialogueEntry* GetDialogueByID(int32 DialogueID);
	// 다음 대화로 진행
	UFUNCTION(BlueprintCallable, Category = Dialogue)
	void ProgressDialogue(int32 NextID);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_ProgressDialogue(int32 NextID);
	
	// 대화 종료
	UFUNCTION(BlueprintCallable, Category = Dialogue)
	virtual void EndDialogue();
	// 대화 중인지 확인
	UFUNCTION(BlueprintCallable, Category = Dialogue)
	FORCEINLINE bool IsInDialogue() const {return CurrentDialogueID >= 0;}
	
	
};
