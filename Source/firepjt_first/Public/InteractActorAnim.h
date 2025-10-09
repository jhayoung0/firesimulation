// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "InteractActorAnim.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UInteractActorAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class AInteractActor* pawnOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Isinteracting = false; 
	
};
