// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "firepjt_firstCharacter.h"
#include "PhoneWidget.h"
#include "Cubee/HousePlayerState.h"
#include "PeopleBase.generated.h"

class AASequencePlayer;
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

	// house ps
	AHousePlayerState* housePs = Cast<AHousePlayerState>(this->GetPlayerState());
	
	// crawl
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Input)
	class UInputAction* crawlInput;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)    
	bool IsCrawl = false;
	
	
	
	// 물건 상호작용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Input)
	class UInputAction* InteractionInput;
	
	// 초기 카메라 위치
	FVector CameraLocation = FVector(2.000000,-30.000000,20.000000);
	FRotator CamerRotation = FRotator(68.000000,-75.000000,-160.000000);
	
	// 기고 있을 때 카메라 위치
	FVector CameramLocationCrawl = FVector(3.089440,-44.742204,-14.281635);
	FRotator CamerRotationCrawl = FRotator(7.716215,-93.557723,-176.070634);

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	TArray<AActor*> allInteractActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	float CanInteractDist = 300.f;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USceneComponent* compActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USceneComponent* compActorMask = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USceneComponent* compActorTowel = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USceneComponent* compActorPeople = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USceneComponent* compActorPeople_first = nullptr;
		
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

	
	// 인터랙팅 액터들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	bool IsInteracting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	bool HasWetTowel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	bool HasMask = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	bool HasPhone = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	bool RescuePeople  = false;

	
protected:
	float PreviousGear = -1.f;
	bool bPreviousHasMask = false;
	bool bPreviousHasWetTowel = false;

public: // widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UMainUI> mainwidget;

	UPROPERTY(Transient)
	UMainUI* mainui = nullptr;


public: // 다음 미션으로 넘기는 함수
	UFUNCTION()
	void GoNextMission();


	// 미션 넘기기
	UFUNCTION()
	void OnCapsuleBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, Category="Mission|Trigger")
	TSet<FName> AllowedProfiles = { FName(TEXT("Mission")) };


public:// 상호작용 액터
	UPROPERTY(EditDefaultsOnly)
	FRotator ActorRotation;

	UPROPERTY(EditDefaultsOnly)
	FVector ActorLocation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic")
	AASequencePlayer* SequenceActor;

public:
	// 서버에서 interacting actor 값이 바뀌면 클라에서 attach actor 호출
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, ReplicatedUsing=AttachActor)
	AInteractActor* InteractingActor = nullptr;

	
	UFUNCTION()
	void CollisionActivate();
	UFUNCTION()
	void Interaction();
	
	// 서버에게 인터랙션 attach or detach 해달라고 요청
	UFUNCTION(Server, Reliable)
	void ServerRPC_Interaction();
	
	// 모든 클라한테 인터랙션 detach 요청 함수
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_DetachActor(AInteractActor* tempActor);

	UFUNCTION()
	void AttachActor();
	void DetachActor(AInteractActor* tempActor);

	UFUNCTION()
	void crawlAction();

	// 변수 동기화
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 인터랙트 대상 찾기
	void FindInteractActor();

	// mask bool 값 하나 두기
	UPROPERTY(EditAnywhere)
	bool HasMaskFirst = false;

	// 산소 고갈
	bool bIsDead = false;
	void OutOfOxygen();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_OutOfOxygen();
	
};
