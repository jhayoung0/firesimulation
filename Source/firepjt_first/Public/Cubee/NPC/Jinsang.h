// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cubee/NPC/NPCBase.h"
#include "Jinsang.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API AJinsang : public ANPCBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOnceTalked = false;
	
	virtual void EndDialogue() override;

public:
	// Walk
	bool IsWalking();
};
