// Fill out your copyright notice in the Description page of Project Settings.


#include "PeopleBase.h"

#include "ASequencePlayer.h"
#include "EnhancedInputComponent.h"
#include "FireMan.h"
#include "firepjt_firstPlayerController.h"
#include "InteractActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "MainUI.h"
#include "Mask.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Trace/Analysis.h"
#include "Cubee/HouseGameMode.h"

APeopleBase::APeopleBase()
{
	//카메라 초기 위치 설정
	GetFirstPersonCameraComponent()->SetRelativeLocation(CameraLocation);
	GetFirstPersonCameraComponent()->SetRelativeRotation(CamerRotation);

	//상호작용 액터 붙일 컴포넌트
	compActor = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingActor"));
	compActor ->SetupAttachment(GetMesh(), TEXT("hand_r"));
	compActor->SetRelativeLocation(FVector(0,40,5.5f));

	//상호작용 액터 붙일 컴포넌트
	compActorMask = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingMask"));
	compActorMask ->SetupAttachment(GetMesh(), TEXT("headsocket"));
	
	compActorTowel = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingTowel"));
	compActorTowel ->SetupAttachment(GetMesh(), TEXT("headsocket2"));

	compActorPeople = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingPeople"));
	compActorPeople_first = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingPeople_first"));
		
	compActorPeople ->SetupAttachment(GetMesh(), TEXT("SpineSocket"));
	compActorPeople_first ->SetupAttachment(GetFirstPersonMesh(), TEXT("SpineSocket_first"));

	compActorJinsang = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingJinsang"));
	compActorJinsang ->SetupAttachment(GetMesh(), TEXT("JinsangSocket"));

	compActorJinsang_first = CreateDefaultSubobject<USceneComponent>(TEXT("InteractingJinsang_first"));
	compActorJinsang_first ->SetupAttachment(GetFirstPersonMesh(), TEXT("JinsangSocket_First"));
	
	
	// 캡슐 hit	
	UCapsuleComponent* Cap = GetCapsuleComponent();
	Cap->SetGenerateOverlapEvents(true);
	
}

void APeopleBase::BeginPlay()
{
	Super::BeginPlay();

	// 콜리젼 이벤트 바인딩
	if (UCapsuleComponent* Cap = GetCapsuleComponent())
	{
		Cap->OnComponentBeginOverlap.AddDynamic(this, &APeopleBase::OnCapsuleBeginOverlap);
	}

	// sequence actor 찾기
	SequenceActor = Cast<AASequencePlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AASequencePlayer::StaticClass()));
	
	// house ps 지정
	housePs = Cast<AHousePlayerState>(this->GetPlayerState());
	
	
	// 레벨에 있는 모든 상호작용 액터를 찾자.
	FindInteractActor();


	// Main ui를 만들자
	if (mainwidget && IsLocallyControlled()) // 소방관에겐 안보이게 하기.
	{
		mainui = CreateWidget<UMainUI>(GetWorld(), mainwidget);
		if (mainui)
		{
			mainui->AddToViewport();
			currOxygen = maxOxygen;
			const float InitPercent = maxOxygen > 0.f ? currOxygen / maxOxygen : 0.f;
			mainui->SetOxygenPercent(InitPercent);
			// 서브미션 설정
			mainui->HandleMission(1);
		}
	}
}


void APeopleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput)
	{
		PlayerInput->BindAction(crawlInput, ETriggerEvent::Started, this, &APeopleBase::crawlAction);
		PlayerInput->BindAction(InteractionInput, ETriggerEvent::Started, this, &APeopleBase::Interaction);
		PlayerInput->BindAction(MaskInput, ETriggerEvent::Started, this, &APeopleBase::InteractWithMask);
	}
}

void APeopleBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 시네마틱 재생 중에 산소 떨어지는거 막기
	if (SequenceActor && SequenceActor->IsPlayingCinematic) {return;}
	
	// 플레이어 죽음 처리
	if (currOxygen <= 0.0f && !bIsDead)
	{
		bIsDead = true;

		if (IsLocallyControlled())
		{
			if (HasAuthority())
			{
				OutOfOxygen();
			}
			else
			{
				ServerRPC_OutOfOxygen();
			}
		}
	}
	
	// 상태별 계수 먼저 갱신
	Posture = IsCrawl ? 0.8f : 1.0f;

	if (HasMask) {
		Gear = 0.4f;
	}
	else if (HasWetTowel) {
		Gear = 0.8f;
	}
	else {
		Gear = 1.0f;
	}

	if (FMath::Abs(Gear - PreviousGear) > KINDA_SMALL_NUMBER || HasMask != bPreviousHasMask || HasWetTowel != bPreviousHasWetTowel)
	{
		UE_LOG(LogTemp, Log, TEXT("Oxygen gear updated - Gear: %.2f, HasMask: %s, HasWetTowel: %s"),
			Gear,
			HasMask ? TEXT("true") : TEXT("false"),
			HasWetTowel ? TEXT("true") : TEXT("false"));
		PreviousGear = Gear;
		bPreviousHasMask = HasMask;
		bPreviousHasWetTowel = HasWetTowel;
	}

	// 산소 소모 계산
	const float OxygenDrain = BaseRate * Posture * Gear * DeltaSeconds;
	currOxygen -= OxygenDrain;
	

	// 0 이하 방지
	currOxygen = FMath::Clamp(currOxygen, 0.0f, maxOxygen);

	const float percent = maxOxygen > 0.f ? currOxygen / maxOxygen : 0.f;

	// 프로그레스 바 업뎃
	if (mainui)
	{
		mainui->SetOxygenPercent(percent);
	}

	// 산소 부족 UI
	if (currOxygen < 30.f)
	{
		if (IsLocallyControlled())
		{
			mainui->ShowDamageUI(0.5f);
		}
		
	}
}

void APeopleBase::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// replicate할 변수 등록
	DOREPLIFETIME(APeopleBase, InteractingActor);
	
}

// c 키  : 기기 
void APeopleBase::crawlAction()
{
	if (!IsCrouched())
		Crouch();
	else
		UnCrouch();
}


// 상호작용 관련
// 상호작용 액터 찾기
void APeopleBase::FindInteractActor()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractActor::StaticClass(), allInteractActor);
}

// e 키
void APeopleBase::Interaction()
{
	// 서버한테 인터랙션 요청
	ServerRPC_Interaction();
}


// 서버에게 인터랙션 attach or detach 해달라고 요청
void APeopleBase::ServerRPC_Interaction_Implementation()
{
	if (IsInteracting)
	{
		AInteractActor* tempActor = InteractingActor;
		
		// 멀티 캐스트로 변경
		MultiCastRPC_DetachActor(tempActor);
		return;
	}

	// 스폰된 마스크 다시 검색
	FindInteractActor();
	
	// 거리가 가까운 상호작용 가능 물체를 변수에 담고
	// 거리가 가까운 상호작용 가능 물체를 변수에 담고
	// 가장 거리가 가까운 상호작용 가능 물체의 상호작용 함수를 호출해준다.
	if (allInteractActor.Num() == 0)
	{
		return;
	}

	float MinDist = CanInteractDist; // 어차피 이 거리보다 크면 체크안하니까 
	float ActorDist = 0.f;
	int32 ClosestIndex = -1;
	for (int32 i = 0; i < allInteractActor.Num(); i++) 
	{
		ActorDist	=  (GetActorLocation() - allInteractActor[i]->GetActorLocation()).Length();
		if (ActorDist < MinDist)
		{
			MinDist = ActorDist;
			ClosestIndex = i;
		}
	}
	
	if (ClosestIndex != -1) {
		// 여기서 인터랙팅 액터가 변경되어서 클라에서 attachactor가 호출됨.
		InteractingActor = Cast<AInteractActor>(allInteractActor[ClosestIndex]);
		// 검색된 액터의 상호작용 함수를 호출
		if (InteractingActor) {
			AttachActor();
		}
	}
}


// 모든 클라한테 detach 요청
void APeopleBase::MultiCastRPC_DetachActor_Implementation(
	AInteractActor* tempActor)
{
	DetachActor(tempActor);
}

void APeopleBase::AttachActor()
{
	ActorRotation = InteractingActor->GetActorRotation();


	if (InteractingActor)
	{
		// tag에 따라서 구분하자.
		if (InteractingActor->ActorHasTag(FName("WetTowel")))
		{
			// mask 가지고 있으면 못씀
			if (HasMask) {return;}

			IsInteracting = true;
			InteractingActor->ToggleWidget(false);
			
			// compActor에 붙이자.
			InteractingActor->AttachToComponent(compActorTowel, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
			HasWetTowel = true;
			HasMask = false;

			InteractingActor->ChangeTowel(true);

			if (IsLocallyControlled())
			{
				if (mainui)
				{
					// 물수건 정보성 UI
					mainui->AddInfoUI(1);
					
					if (mainui->CurrentSubMission == 4)
					{
						mainui->SuccessSubMission();
					}
					else if (mainui->CurrentSubMission < 4)
					{
						mainui->SubMissionWarning();
					}
				}
			
			}
		}
		else if (InteractingActor->ActorHasTag(FName("Phone")))
		{
			// 미션 전에 하면 핸드폰은 아예 attach 가 안 됨.
			if (IsLocallyControlled())
			{
				// 두번째 미션 이전에 하려고 하면 워닝 뜨게 하기
				if (mainui->CurrentSubMission < 2)
				{
					mainui->SubMissionWarning();
				}
				else
				{
					IsInteracting = true;
					InteractingActor->ToggleWidget(false);
			
					// compActor에 붙이자.
					InteractingActor->AttachToComponent(compActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					HasPhone = true;
					
					// 위젯 띄우기
					auto* pc = Cast<Afirepjt_firstPlayerController>(GetWorld()->GetFirstPlayerController());
					pc->OpenPhoneUI();

					// 핸드폰 정보성 UI
					mainui->AddInfoUI(0);
				}
				
				
			}
		}
		else if (InteractingActor->ActorHasTag(FName("People")))
		{
			IsInteracting = true;
			InteractingActor->ToggleWidget(false);
			
			if (IsLocallyControlled())
			{
				// 사람 정보성 UI
				mainui->AddInfoUI(2);
			}
			
			if (!HasAuthority())
			{
				// 남이 보는 시선
				// compActor에 붙이자.
				InteractingActor->AttachToComponent(compActorPeople, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				RescuePeople = true;
			}
			else
			{
				// 자기가 보는 시선
				// compActor에 붙이자.
				InteractingActor->AttachToComponent(compActorPeople_first, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				RescuePeople = true;
			}

			// 서브미션 할 차례가 아니면 워닝 뜨게 하자.
			if (mainui && mainui->CurrentSubMission < 3)
			{
				mainui->SubMissionWarning();
			}
	
		}
		else if (InteractingActor->ActorHasTag(FName("Door")))
		{
			InteractingActor->ToggleDoor();
			IsInteracting = true;
			InteractingActor->ToggleWidget(false);
			
		}
		else
		{
			IsInteracting = true;
			InteractingActor->ToggleWidget(false);
			// compActor에 붙이자.
			InteractingActor->AttachToComponent(compActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Log, TEXT("태그 없음 또는 알 수 없는 타입"));
		}
	}
}


void APeopleBase::DetachActor(AInteractActor* tempActor)
{
	IsInteracting = false;
	InteractingActor->ToggleWidget(true);
	
	
	// 분리하자
	if (tempActor)
	{
		tempActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		tempActor->SetActorRotation(ActorRotation);
	}
	

	
	// tag에 따라서 구분하자.
	if (tempActor)
	{
		if (tempActor->ActorHasTag(FName("WetTowel")))
		{
			
			HasWetTowel = false;
			InteractingActor->ChangeTowel(false);
		}
		else if (tempActor->ActorHasTag(FName("Phone")))
		{
			if (IsLocallyControlled())
			{
				auto* pc = Cast<Afirepjt_firstPlayerController>(GetWorld()->GetFirstPlayerController());
				pc->ClosePhoneUI();
			}
			HasPhone = false;
			
		}
		else if (tempActor->ActorHasTag(FName("Door")))
		{
			InteractingActor->ToggleDoor();
		}
		else if (tempActor->ActorHasTag(FName("People")))
		{
			RescuePeople = false;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("태그 없음 또는 알 수 없는 타입"));
		}
	}
	InteractingActor = nullptr;
}



// mask랑 상호작용
void APeopleBase::InteractWithMask()
{
	// 젖은 타월과 상호작용하고 있으면 호출 안됨.
	if (HasWetTowel) {return;}
	
	// 이미 마스크를 가지고 있다면 detach 아니면 attach
	if (HasMask)
	{
		ServerRPC_DetachMask();
	}
	else
	{
		ServerRPC_AttachMask();
	}
}

void APeopleBase::ServerRPC_AttachMask_Implementation()
{
	
	// 모든 클라에서 마스크 붙이기 요청
	NetmultiCastRPC_AttachMask();
}

void APeopleBase::NetmultiCastRPC_AttachMask_Implementation()
{
	
	// mask 를 찾기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMask::StaticClass(), MaskActors);
	// 찾은 mask actor들과의 모든 거리를 구하기.
	float dist = 200;
	
	for (int32 i = 0; i < MaskActors.Num(); i++)
	{
		// 거리 구하기 
		if (FVector::Dist(MaskActors[i]->GetActorLocation(), this->GetActorLocation()) < dist)
		{
			// 거리가 더 작으면 업데이트
			dist = FVector::Dist(MaskActors[i]->GetActorLocation(), this->GetActorLocation());
			MaskActor = Cast<AMask>(MaskActors[i]);
		}
	}

	// 가장 가까운 mask actor 못찾으면 리턴
	if (MaskActor==nullptr) {return;}
	// 찾았으면 회전값 저장해두기
	MaskActorRotation = MaskActor->GetActorRotation();
	// has mask 설정해주기
	HasMask = true;
	HasWetTowel = false;

	// 미션 넘기기
	if (IsLocallyControlled())
	{
		if (mainui && mainui->CurrentSubMission == 5)
		{
			// 서브미션 넘기기
			mainui->SuccessSubMission();
			// 최초 1회만 호출됨. (미션 넘기기)
			if (!HasMaskFirst) {GoNextMission();}
			// 최초 1회를 위한 변수 (1번 바뀌면 값 변경 안됨)
			HasMaskFirst = true;
			
		}
		else if (mainui && mainui->CurrentSubMission < 5)
		{
			mainui->SubMissionWarning();
		}
	}

	// 못찾으면 리턴
	if (!MaskActor) {UE_LOG(LogTemp, Warning, TEXT("maskactor없음")); return;}

	// 찾았으면 시뮬레이터 꺼주고 붙이기
	MaskActor->ToggleMask(true);
	MaskActor->AttachToComponent(compActorMask, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// 소방관에게 hasMask 여부를 알려준다.
	if (HasAuthority())
	{
		if (AFireMan* firemanActor = Cast<AFireMan>(UGameplayStatics::GetActorOfClass(GetWorld(), fireManFactory)))
		{
			firemanActor->CheckMaskToPerson(true);
		}
	}
	
	// mask ui 설정해주기
	if (IsLocallyControlled())
	{
		mainui->ShowMaskUI(true);
		if (masksfx) 
		{
			// AudioComponent를 생성하고 루프 설정
			MaskAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), masksfx, 1.0f, 1.0f, 0.0f, nullptr, true);
		}
	}
}

void APeopleBase::ServerRPC_DetachMask_Implementation()
{
	NetmultiCastRPC_DetachMask();
}


void APeopleBase::NetmultiCastRPC_DetachMask_Implementation()
{
	HasMask = false;
	// 분리하자
	if (MaskActor)
	{
		MaskActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		MaskActor->SetActorRotation(MaskActorRotation);
		MaskActor->ToggleMask(false);
	}
	if (IsLocallyControlled())
	{
		// mask ui 끄기
		mainui->ShowMaskUI(false);
		// 마스크 sound 끄기
		if (MaskAudioComp && MaskAudioComp->IsPlaying())
		{
			MaskAudioComp->Stop();
		}
	}
	// mask actor 다시 null 처리
	MaskActor = nullptr;
}



// 다음 미션으로 넘기기
void APeopleBase::GoNextMission()
{
	
	if (!housePs){return;}
	housePs->SetMissionComplete();
}

// 마지막 미션 및 첫번째 서브미션 완료 처리 조건
void APeopleBase::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!OtherActor || !OtherComp) return;
	if (OtherActor == this) return; // 자기 자신 무시

	// 원하는 Collision Profile과 일치하는지 체크
	const FName Profile = OtherComp->GetCollisionProfileName();

	if (AllowedProfiles.Contains(Profile))
	{
		// 첫번째 서브미션 성공
		if (OtherActor->ActorHasTag(FName("OneSubMission")))
		{
			if (mainui)
			{
				UE_LOG(LogTemp,Warning,TEXT("%d"), mainui->CurrentSubMission);
				if (mainui->CurrentSubMission == 1)
				{
					if (IsLocallyControlled())
					{
						// 서브미션 성공
						mainui->SuccessSubMission();
					}
				}
			}
		
		}
		
		// 다음 미션으로 가기 / 마지막 미션임.
		if (OtherActor->ActorHasTag(FName("MainLastMission")))
		{
			UE_LOG(LogTemp,Warning,TEXT("MainLastMission"));

			if (mainui && mainui->CurrentSubMission == 6)
			{
				this->GoNextMission();
				// 산소바 ui 삭제하기
				if (mainui)
				{
					mainui->RemoveFromParent();
					// 서브미션 성공
					mainui->SuccessSubMission();
				
				}
			}
			else if (mainui && mainui->CurrentSubMission < 6)
			{
				// 서브미션 순서대로 해라!
				mainui->SubMissionWarning();
			}	
		}
	}
}



// 죽음 처리
void APeopleBase::OutOfOxygen()
{
	AHouseGameMode* GM = Cast<AHouseGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->FailMission();
	}
}


void APeopleBase::ServerRPC_OutOfOxygen_Implementation()
{
	OutOfOxygen();
}



// NPC 상호작용
void APeopleBase::AttachJinsang()
{
	// 진상 찾을래
	jinsang = Cast<AJinsang>(UGameplayStatics::GetActorOfClass(GetWorld(), AJinsang::StaticClass()));
	
	if (IsLocallyControlled())
	{
		// 진상 붙일래 (1인칭)
		jinsang->AttachToComponent(compActorJinsang_first, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		// 남이 보는 시선
		// 진상 붙일래
		jinsang->AttachToComponent(compActorJinsang, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	
}

	


void APeopleBase::ServerRPC_AttachJinsang_Implementation()
{
	MultiCastRPC_AttachJinsang();
}

void APeopleBase::MultiCastRPC_AttachJinsang_Implementation()
{
	AttachJinsang();
}
