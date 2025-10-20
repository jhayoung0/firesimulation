// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FiremanAnim.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API UFiremanAnim : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY()
	TObjectPtr<class AFireMan> pawnOwner;

	// Movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Move)
	FVector Velocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Move)
	float Horizontal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Move)
	float Vertical;

	// Tool
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tool)
	bool bDoesEquipFireHose = false;
};
