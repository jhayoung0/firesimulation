// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireTruckFireHose.generated.h"

UCLASS()
class FIREPJT_FIRST_API AFireTruckFireHose : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireTruckFireHose();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> FireHoseComp;
};
