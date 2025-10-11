// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "firepjt_firstCharacter.h"
#include "PeopleBase.generated.h"

class USceneComponent;
class UMainUI;
class AInteractActor;
class AActor;

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
	virtual void Tick(float DeltaSeconds) override;
	
	// crawl
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Input)
	class UInputAction* crawlInput;

	UFUNCTION()
	void crawlAction();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsCrawl = false;

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



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	TArray<AActor*> allInteractActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	float CanInteractDist = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	AInteractActor* InteractingActor = nullptr;

	void AttachActor();
	void DetachActor(AInteractActor* tempActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USceneComponent* compActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USceneComponent* compActorMask = nullptr;
	
public: // stat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float maxOxygen = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	float currOxygen = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float BaseRate = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	float Posture = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	float Gear = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	bool IsInteracting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	bool HasWetTowel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	bool HasMask = false;

protected:
	float PreviousGear = -1.f;
	bool bPreviousHasMask = false;
	bool bPreviousHasWetTowel = false;

public: // widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UMainUI> mainwidget;

	UPROPERTY(Transient)
	UMainUI* mainui = nullptr;


public:// 상호작용 액터
	UPROPERTY(EditDefaultsOnly)
	FRotator ActorRotation;

	UPROPERTY(EditDefaultsOnly)
	FVector ActorLocation;
	
};
