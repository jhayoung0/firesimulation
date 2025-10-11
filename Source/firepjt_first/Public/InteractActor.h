// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractActor.generated.h"

UCLASS()
class FIREPJT_FIRST_API AInteractActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public: // 인터랙트 컴포넌트 붙이기 (아웃라인, 가까이 갔을 경우 UI 활성화)


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* meshComp;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UInteractWidgetComp* InteractWidgetComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* InteractMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInteracting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInteractWidget* InteractUI;

	
	UFUNCTION() // 상호작용 몽타주 재생
	void PlayInteract();

	UFUNCTION() // 위젯 노출/미노출
	void ToggleWidget(bool check);
};

