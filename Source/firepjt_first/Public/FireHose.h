// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "FireHose.generated.h"

UCLASS()
class FIREPJT_FIRST_API AFireHose : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

public:
	AFireHose();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> SceneRootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMeshComponent> FirehoseComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> NiagaraParticleSystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCableComponent> HoseComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAudioComponent> FireHoseSound;

	// FireTruck FireHose
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AFireTruckFireHose> FireTruckFireHoseClass;

public:
	// Water Shot
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void OnWaterShot();
	
	UFUNCTION(BlueprintCallable)
	void OnDirectShotMode();

	UFUNCTION(BlueprintCallable)
	void OnSprayShotMode();

	bool GetDoesWaterShotNow();

	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

private:
	UPROPERTY(Replicated)
	bool bDoesWaterShotNow = false;
	UPROPERTY(Replicated)
	bool bDoesSprayShotNow = false;
	UPROPERTY(Replicated)
	bool bDoesDirectShotNow = false;
};
