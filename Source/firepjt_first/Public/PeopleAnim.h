// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PeopleAnim.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UPeopleAnim : public UAnimInstance
{
	GENERATED_BODY()


public:
	// 소유 Pawn
	UPROPERTY()
	class APeopleBase* pawnOwner;

	//이동속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector velocity;

	// 공중에 있는지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool isinAir;

	// 기고 있는지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool isCrawl;
	
	// 수평 이동 속력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float groundSpeed;

	// 좌우 이동에 따른 값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float dirH;
	// 앞뒤 이동에 따른 값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float dirV;
	

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
};
