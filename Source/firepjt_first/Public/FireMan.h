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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USpringArmComponent> FiremanSpringArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCameraComponent> FiremanCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USceneComponent> Person2Pos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USceneComponent> FirehosePos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UChildActorComponent> WaterComp;

	// FireHose
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AFireHose> FireHoseActor;
	UPROPERTY(Replicated)
	bool bDoesEquipFireHose = false;
	
	// Crowbar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> CrowbarMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AInteractActor> DoorClass;
	UPROPERTY()
	TObjectPtr<AInteractActor> DoorActor;
	UPROPERTY(Replicated)
	bool bDoesEquipCrowbar = false;

	// Carry Person
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AInteractActor> Person2Class;
	UPROPERTY()
	TObjectPtr<AInteractActor> Person2Actor;
	FRotator ActorRotation;
	UPROPERTY(Replicated)
	bool bDoesCarryingPerson = false;

	// Mask
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AMask> MaskActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaskSpawnDist = 100.f;

	// Interact
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractDist = 200.f;

	// Get
	float GetRotationSpine02();
	UFUNCTION()
	void CheckMaskToPerson(bool bPersonHasMask);
	bool GetBHasMask();
	
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
	UFUNCTION(Server, Unreliable)
	void ServerRPC_AddPitchInputToSpine(float pitch);

	// Change bCanUse~ when Replicated
	UFUNCTION()
	void OnChangeCanUseTool();
	
	// FireHose
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AFireTruckFireHose> FireTruckFireHoseClass;
	UPROPERTY()
	TObjectPtr<class AFireTruckFireHose> FireTruckFireHoseActor;
	UPROPERTY(Replicated)
	bool bCanUseFireHose = false;
	void OnGetFireHose();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnGetFireHose();
	UFUNCTION(Client, Reliable)
	void ClientRPC_OnGetFireHose();
	
	void OnEquipFireHose();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnEquipFireHose();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnEquipFireHose();
	
	// Crowbar
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AFireTruckCrowbar> FireTruckCrowbarClass;
	UPROPERTY()
	TObjectPtr<class AFireTruckCrowbar> FireTruckCrowbarActor;
	UPROPERTY(Replicated)
	bool bCanUseCrowbar = false;
	void OnGetCrowbar();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnGetCrowbar();
	UFUNCTION(Client, Reliable)
	void ClientRPC_OnGetCrowbar();
	
	void OnEquipCrowbar();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnEquipCrowbar();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnEquipCrowbar();

	// FireHose Shot
	void OnFireHoseShot();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnFireHoseShot();

	// Use Tool
	void OnUseTool();
	// Mask Out
	UPROPERTY(Replicated)
	bool bHasMask = false;
	void OnMaskOut();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnMaskOut();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnMaskOut();
	// Open Door
	UFUNCTION(Server, Reliable)
	void ServerRPC_OpenDoor();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OpenDoor();
	// Carry Person
	UFUNCTION(Server, Reliable)
	void ServerRPC_CarryPerson();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CarryPerson();

	// Movement Variables
	FVector direction;
	float WalkSpeed;

	// Rotation Variable
	UPROPERTY(Replicated)
	float Rotation_Spine02;

	// FireHose Off
	void OffFireHose();

	// Mission Complete
	UPROPERTY(EditAnywhere)
	int32 MaxMissionIndex = 3;
	int32 CurrentMissionIndex = 1;
	void OnMissionComplete();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnMissionComplete();

	FName SafeZoneCollisionProfileName = FName(TEXT("Mission"));
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp,	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// UI
	UPROPERTY(VisibleAnywhere, Category = UI)
	TSubclassOf<class UFiremanMainUI> FireManMainUIClass;

	UPROPERTY(Transient)
	TObjectPtr<class UFiremanMainUI> FireManMainUIWidget;

public:
	// NPC Interaction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDetected = false;
};
