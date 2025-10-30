// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mask.generated.h"

UCLASS()
class FIREPJT_FIRST_API AMask : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMask();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* meshComp;

	UFUNCTION()
	void ToggleMask(bool check);
};
