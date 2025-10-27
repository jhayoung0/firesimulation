// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireActor.generated.h"

UCLASS()
class FIREPJT_FIRST_API AFireActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> FireComp;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TObjectPtr<class UPointLightComponent> FireLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UInteractWidgetComp> InteractWidgetComp;
	
	void PutOutFire();

protected:
	// Fire Vanish Rate Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PutOutFire)
	float vanishAlpha = 0.2f;

	// PutOutFire
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NiagaraVariable|PutOutFire")
	float FlameScale = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NiagaraVariable|PutOutFire")
	float SmokeScale = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NiagaraVariable|PutOutFire")
	float DistortionScale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|PutOutFire")
	float DebrisRate = 80.f;
	// Flame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|Flame")
	float FlameSpawnRate = 320.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|Flame")
	float FlameRadius = 35.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|Flame")
	float FlameVelocity = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|Flame")
	float FlameConeAngle = 64.f;
	// Smoke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|Smoke")
	float SmokeSpriteSizeMin = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|Smoke")
	float SmokeSpriteSizeMax = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|Smoke")
	float SmokeLifetime = 3.f;
	// Fire Debris
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|FireDebris")
	float DebrisVelocity = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|FireDebris")
	float DebrisConeAngle = 64.f;
	// Heat Distortion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|HeatDistortion")
	float DistortionSpawnRate = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|HeatDistortion")
	float DistortionRadius = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|HeatDistortion")
	float DistortionVelocity = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NiagaraVariable|HeatDistortion")
	float DistortionConeAngle = 48.f;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> SceneRootComp;

	void BillboardInteractKey();
};
