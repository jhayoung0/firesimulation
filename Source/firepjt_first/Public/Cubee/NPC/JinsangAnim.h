// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JinsangAnim.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UJinsangAnim : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	class AJinsang* OwningPawn;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsTalking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsOnceTalked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsWalking;
};
