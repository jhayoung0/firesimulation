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

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> SceneRootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> NiagaraParticleSystemComp;

	UFUNCTION(BlueprintCallable)
	void OnWaterShot();
	
	UFUNCTION(BlueprintCallable)
	void OnDirectShotMode();

	UFUNCTION(BlueprintCallable)
	void OnSprayShotMode();

	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;
};
