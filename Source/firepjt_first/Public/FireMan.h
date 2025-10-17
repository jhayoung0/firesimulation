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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCameraComponent> FiremanCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USceneComponent> FirehosePos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UChildActorComponent> WaterComp;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputMappingContext> FiremanIMC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> InteractAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> AxeAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> FireHoseAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MaskOutAction;

	void OnMove(const struct FInputActionValue& value);
	void OnLook(const struct FInputActionValue& value);
	void OnInteract(const struct FInputActionValue& value);
	void OnAxe(const struct FInputActionValue& value);
	void OnFireHose(const struct FInputActionValue& value);
	void OnMaskOut(const struct FInputActionValue& value);

	FVector direction;
	float WalkSpeed;
	
};
