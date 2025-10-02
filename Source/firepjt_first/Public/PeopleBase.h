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

public: 

	// 생성자
	APeopleBase();
	virtual void BeginPlay() override;

	// 인풋
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//virtual void Tick(float DeltaSeconds) override;
	
	// crawl
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Input)
	class UInputAction* crawlInput;

	UFUNCTION()
	void crawlAction();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsCrawl;

	// 기고 있을 때 속도 느리게 설정
	void ApplyCrawlState(bool bEnable);
	
	// 물건 상호작용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Input)
	class UInputAction* InteractionInput;

	// 초기 카메라 위치
	FVector CameraLocation = FVector(2.000000,-30.000000,20.000000);
	FRotator CamerRotation = FRotator(68.000000,-75.000000,-160.000000);
	
	// 기고 있을 때 카메라 위치
	FVector CameramLocationCrawl = FVector(3.089440,-44.742204,-14.281635);
	FRotator CamerRotationCrawl = FRotator(7.716215,-93.557723,-176.070634);

	UFUNCTION()
	void Interaction();


public: // stat
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 maxOxygen = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 currOxygen = maxOxygen;
	
};
