// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FireMan.generated.h"

UCLASS()
class FIREPJT_FIRST_API AFireMan : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFireMan();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCameraComponent> FiremanCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USceneComponent> FirehosePos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UChildActorComponent> WaterComp;

	// FireHose
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AFireHose> FireHoseActor;
	bool bDoesEquipFireHose = false;
	
	// Crowbar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> CrowbarMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AInteractActor> DoorClass;
	UPROPERTY()
	TObjectPtr<AActor> DoorActor;
	bool bDoesEquipCrowbar = false;

	// Carry Person
	bool bDoesCarryingPerson = false;

	// Mask
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AInteractActor> MaskActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaskSpawnDist = 100.f;

	// Interact
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractDist = 1000.f;
	
private:
	// Animation Blueprint
	UPROPERTY()
	TObjectPtr<class UFiremanAnim> FiremanAnimInstance;
	
	// Inputs
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputMappingContext> FiremanIMC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> EquipFireHoseAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> EquipCrowbarAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> FireHoseAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> UseToolAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MaskOutAction;

	void OnMove(const struct FInputActionValue& value);
	void OnLook(const struct FInputActionValue& value);
	void OnEquipFireHose();
	void OnEquipCrowbar();
	void OnFireHoseShot();
	void OnUseTool();
	void OnMaskOut();
	void OnCarryPerson();

	// Movement Variables
	FVector direction;
	float WalkSpeed;

	// FireHose Off
	void OffFireHose();
};
