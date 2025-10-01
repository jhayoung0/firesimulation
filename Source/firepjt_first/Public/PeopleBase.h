// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "firepjt_firstCharacter.h"
#include "PeopleBase.generated.h"

/**
 * 
 */
UCLASS()
class FIREPJT_FIRST_API APeopleBase : public Afirepjt_firstCharacter
{
	GENERATED_BODY()

public: // 인풋

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//virtual void Tick(float DeltaSeconds) override;
	
	// crawl
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* crawlInput;

	UFUNCTION()
	void crawlAction();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsCrawl;
	
	// 물건 상호작용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* InteractionInput;
	
	UFUNCTION()
	void Interaction();


	
};
