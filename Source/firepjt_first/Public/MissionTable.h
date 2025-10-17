// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "MissionTable.generated.h"
/**
 * 
 */

class ULevelSequencePlayer;

USTRUCT(BlueprintType)
struct FMissionTable: public FTableRowBase
{
	GENERATED_BODY();
	
	FMissionTable();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName mission_id; // 1,2,3 // 1: 미션 1, 2: 미션 2, 3: 미션3
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText mission_head; // 미션 설명

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText mission_description; // 미션 설명
	
};
