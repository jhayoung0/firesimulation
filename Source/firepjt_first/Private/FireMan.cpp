// Fill out your copyright notice in the Description page of Project Settings.


#include "FireMan.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FireHose.h"
#include "FiremanAnim.h"
#include "FiremanMainUI.h"
#include "FireTruckCrowbar.h"
#include "FireTruckFireHose.h"
#include "InteractActor.h"
#include "MainUI.h"
#include "PeopleBase.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Cubee/NPC/NPCBase.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AFireMan::AFireMan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFireMan::OnCapsuleBeginOverlap);
	
	// Fireman Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> firemanMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/CHARACTERS/PLAYER/B-01_TACTICAL/fix_v2/SKM_B-01_v4_BRAWNY_SIMPLE.SKM_B-01_v4_BRAWNY_SIMPLE'"));
	if (firemanMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(firemanMeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));

	// Camera Collision
	FiremanSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FiremanSpringArm"));
	FiremanSpringArm->SetupAttachment(GetMesh(), TEXT("head"));
	FiremanSpringArm->SetRelativeLocationAndRotation(FVector(9.396923,3.420199,-0), FRotator(0, 20, -90));
	FiremanSpringArm->TargetArmLength = 0.f;

	// Fireman Camera
	FiremanCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FiremanCamera"));
	FiremanCamera->SetupAttachment(FiremanSpringArm);
	FiremanCamera->bUsePawnControlRotation = true;

	// Person2 Attach Position
	Person2Pos = CreateDefaultSubobject<USceneComponent>(TEXT("Person2Pos"));
	Person2Pos->SetupAttachment(GetMesh());
	Person2Pos->SetRelativeLocationAndRotation(FVector(-6,57,54.5), FRotator(0, 0, 10));

	// Water Niagara System
	WaterComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("WaterComp"));
	ConstructorHelpers::FClassFinder<AFireHose> WaterRef(TEXT("/Game/CustomContents/Fireman/Water/BP_FireHose.BP_FireHose_C"));
	if (WaterRef.Succeeded())
	{
		WaterComp->SetChildActorClass(WaterRef.Class);
		WaterComp->SetupAttachment(GetMesh(), TEXT("cc_weaponbone_r"));
		WaterComp->SetVisibility(false);
		WaterComp->SetRelativeLocationAndRotation(FVector(-2.365337,-7.737841,-0.118154), FRotator(8.503906,-79.604077,259.078693));
	}

	// Crowbar Mesh
	CrowbarMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CrowbarMeshComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> crowbarRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Fab/Crowbar_Low-poly/crowbar/StaticMeshes/SKM_crowbar.SKM_crowbar'"));
	if (crowbarRef.Succeeded())
	{
		CrowbarMeshComp->SetSkeletalMesh(crowbarRef.Object);
		CrowbarMeshComp->SetupAttachment(GetMesh(), TEXT("cc_weaponbone_r"));
		CrowbarMeshComp->SetVisibility(false);
		CrowbarMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CrowbarMeshComp->SetRelativeLocationAndRotation(FVector(22.5,-19,10.5), FRotator(19,-97.,-102.5));
	}
	
	// Aniimation Blueprint
	ConstructorHelpers::FClassFinder<UAnimInstance> animInstanceRef(TEXT("/Game/CustomContents/Fireman/Blueprints/ABP_Fireman.ABP_Fireman_C"));
	if (animInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstanceRef.Class);
	}

	// Inputs
	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Fireman.IMC_Fireman'"));
	if (IMCRef.Succeeded())
	{
		FiremanIMC = IMCRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> moveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (moveActionRef.Succeeded())
	{
		MoveAction = moveActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> lookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MouseLook.IA_MouseLook'"));
	if (lookActionRef.Succeeded())
	{
		LookAction = lookActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> equipFireHoseActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/Fireman/IA_EquipFireHose.IA_EquipFireHose'"));
	if (equipFireHoseActionRef.Succeeded())
	{
		EquipFireHoseAction = equipFireHoseActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> equipCrowbarActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/Fireman/IA_EquipCrowbar.IA_EquipCrowbar'"));
	if (equipCrowbarActionRef.Succeeded())
	{
		EquipCrowbarAction = equipCrowbarActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> fireHoseActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/Fireman/IA_FireHose.IA_FireHose'"));
	if (fireHoseActionRef.Succeeded())
	{
		FireHoseAction = fireHoseActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> useToolActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/Fireman/IA_UseTool.IA_UseTool'"));
	if (useToolActionRef.Succeeded())
	{
		UseToolAction = useToolActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> maskOutActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/Fireman/IA_MaskOut.IA_MaskOut'"));
	if (maskOutActionRef.Succeeded())
	{
		MaskOutAction = maskOutActionRef.Object;
	}

	// Person2 Class
	ConstructorHelpers::FClassFinder<AInteractActor> person2ClassRef(TEXT("/Game/CustomContents/People/Blueprints/BP_People_2.BP_People_2_C"));
	if (person2ClassRef.Succeeded())
	{
		Person2Class = person2ClassRef.Class;
	}

	// Door Class
	ConstructorHelpers::FClassFinder<AInteractActor> doorClassRef(TEXT("/Game/CustomContents/People/Blueprints/BP_Door.BP_Door_C"));
	if (doorClassRef.Succeeded())
	{
		DoorClass = doorClassRef.Class;
	}
	
	// Mask Actor
	ConstructorHelpers::FClassFinder<AMask> maskRef(TEXT("/Game/CustomContents/People/Blueprints/BP_Mask.BP_Mask_C"));
	if (maskRef.Succeeded())
	{
		MaskActor = maskRef.Class;
	}

	// FireTruck FireHose
	ConstructorHelpers::FClassFinder<AFireTruckFireHose> fireTruckFireHoseRef(TEXT("/Game/CustomContents/Fireman/FireTruck/BP_FirehoseInTruck.BP_FirehoseInTruck_C"));
	if (fireTruckFireHoseRef.Succeeded())
	{
		FireTruckFireHoseClass = fireTruckFireHoseRef.Class;
	}
	// FireTruck Crowbar
	ConstructorHelpers::FClassFinder<AFireTruckCrowbar> fireTruckCrowbarRef(TEXT("/Game/CustomContents/Fireman/FireTruck/BP_CrowbarInTruck.BP_CrowbarInTruck_C"));
	if (fireTruckCrowbarRef.Succeeded())
	{
		FireTruckCrowbarClass = fireTruckCrowbarRef.Class;
	}

	// FiremanMainUI
	ConstructorHelpers::FClassFinder<UFiremanMainUI> mainUIRef(TEXT("/Game/CustomContents/UI/WBP_FiremanMainUI.WBP_FiremanMainUI_C"));
	if (mainUIRef.Succeeded())
	{
		FireManMainUIClass = mainUIRef.Class;
	}
}

// Called when the game starts or when spawned
void AFireMan::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys)
		{
			subsys->AddMappingContext(FiremanIMC, 0);
		}
	}

	FiremanAnimInstance = Cast<UFiremanAnim>(GetMesh()->GetAnimInstance());
	DoorActor = Cast<AInteractActor>(UGameplayStatics::GetActorOfClass(GetWorld(), DoorClass));
	Person2Actor = Cast<AInteractActor>(UGameplayStatics::GetActorOfClass(GetWorld(), Person2Class));
	FireTruckFireHoseActor = Cast<AFireTruckFireHose>(UGameplayStatics::GetActorOfClass(GetWorld(), FireTruckFireHoseClass));
	FireTruckCrowbarActor = Cast<AFireTruckCrowbar>(UGameplayStatics::GetActorOfClass(GetWorld(), FireTruckCrowbarClass));

	// only fireman can see
	if (FireManMainUIClass && IsLocallyControlled())
	{
		FireManMainUIWidget = CreateWidget<UFiremanMainUI>(GetWorld(), FireManMainUIClass);
		if (FireManMainUIWidget)
		{
			FireManMainUIWidget->AddToViewport();
		}
	}
}

// Called every frame
void AFireMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rot = FRotator::ZeroRotator;
	rot.Yaw = GetControlRotation().Yaw;
	
	direction = FTransform(rot).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void AFireMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		playerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFireMan::OnMove);
		playerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFireMan::OnLook);
		playerInput->BindAction(EquipFireHoseAction, ETriggerEvent::Started, this, &AFireMan::OnEquipFireHose);
		playerInput->BindAction(EquipCrowbarAction, ETriggerEvent::Started, this, &AFireMan::OnEquipCrowbar);
		playerInput->BindAction(FireHoseAction, ETriggerEvent::Started, this, &AFireMan::OnFireHoseShot);
		playerInput->BindAction(UseToolAction, ETriggerEvent::Started, this, &AFireMan::OnUseTool);
		playerInput->BindAction(MaskOutAction, ETriggerEvent::Started, this, &AFireMan::OnMaskOut);
	}
}

void AFireMan::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFireMan, bCanUseFireHose);
	DOREPLIFETIME(AFireMan, bDoesEquipFireHose);
	DOREPLIFETIME(AFireMan, bCanUseCrowbar);
	DOREPLIFETIME(AFireMan, bDoesEquipCrowbar);
	DOREPLIFETIME(AFireMan, bDoesCarryingPerson);
	DOREPLIFETIME(AFireMan, Rotation_Spine02);
}

float AFireMan::GetRotationSpine02()
{
	return Rotation_Spine02;
}

void AFireMan::OnMove(const struct FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	direction.X = v.X;
	direction.Y = v.Y;
}

void AFireMan::OnLook(const struct FInputActionValue& value)
{
	float yaw = value.Get<FVector2D>().X;
	float pitch = value.Get<FVector2D>().Y;
	AddControllerYawInput(yaw);
	ServerRPC_AddPitchInputToSpine(pitch);
}

void AFireMan::ServerRPC_AddPitchInputToSpine_Implementation(float pitch)
{
	Rotation_Spine02 = FMath::Clamp(Rotation_Spine02 + pitch, -30, 90);
}

void AFireMan::OnChangeCanUseTool()
{
	if (bCanUseCrowbar && bCanUseFireHose)
		OnMissionComplete();
}

void AFireMan::OnGetFireHose()
{
	if (bCanUseFireHose) return;
	
	ServerRPC_OnGetFireHose();
}

void AFireMan::ServerRPC_OnGetFireHose_Implementation()
{
	if (FireTruckFireHoseActor)
	{
		float dist = FVector::Distance(GetActorLocation(), FireTruckFireHoseActor->GetActorLocation());
		if (dist <= InteractDist)
		{
			bCanUseFireHose = true;
			OnChangeCanUseTool();
			ClientRPC_OnGetFireHose();
			Multicast_OnEquipFireHose();
		}
	}
}

void AFireMan::ClientRPC_OnGetFireHose_Implementation()
{
	FireManMainUIWidget->AddInfoUI(0);
}

void AFireMan::OnEquipFireHose()
{
	ServerRPC_OnEquipFireHose();
}

void AFireMan::ServerRPC_OnEquipFireHose_Implementation()
{
	if (!bCanUseFireHose || bDoesCarryingPerson) return;

	Multicast_OnEquipFireHose();
}

void AFireMan::Multicast_OnEquipFireHose_Implementation()
{
	if (bDoesEquipFireHose)
	{
		// off
		OffFireHose();
		bDoesEquipFireHose = false;
		WaterComp->SetVisibility(false);
	}
	else
	{
		// crowbar off
		if (bDoesEquipCrowbar)
		{
			bDoesEquipCrowbar = false;
			CrowbarMeshComp->SetVisibility(false);
		}
		// firehose on
		bDoesEquipFireHose = true;
		WaterComp->SetVisibility(true);
	}
}

void AFireMan::OnGetCrowbar()
{
	if (bCanUseCrowbar) return;

	ServerRPC_OnGetCrowbar();
}

void AFireMan::ServerRPC_OnGetCrowbar_Implementation()
{
	if (FireTruckCrowbarActor)
	{
		float dist = FVector::Distance(GetActorLocation(), FireTruckCrowbarActor->GetActorLocation());
		if (dist <= InteractDist)
		{
			bCanUseCrowbar = true;
			FireTruckCrowbarActor->Destroy();
			OnChangeCanUseTool();
			ClientRPC_OnGetCrowbar();
			Multicast_OnEquipCrowbar();
		}
	}
}

void AFireMan::ClientRPC_OnGetCrowbar_Implementation()
{
	FireManMainUIWidget->AddInfoUI(1);
}

void AFireMan::OnEquipCrowbar()
{
	ServerRPC_OnEquipCrowbar();
}

void AFireMan::ServerRPC_OnEquipCrowbar_Implementation()
{
	if (!bCanUseCrowbar || bDoesCarryingPerson) return;
	
	Multicast_OnEquipCrowbar();
}

void AFireMan::Multicast_OnEquipCrowbar_Implementation()
{
	if (bDoesEquipCrowbar)
	{
		// off
		bDoesEquipCrowbar = false;
		CrowbarMeshComp->SetVisibility(false);
	}
	else
	{
		// firehose off
		if (bDoesEquipFireHose)
		{
			OffFireHose();
			bDoesEquipFireHose = false;
			WaterComp->SetVisibility(false);
		}
		// crowbar on
		bDoesEquipCrowbar = true;
		CrowbarMeshComp->SetVisibility(true);
	}
}

void AFireMan::OnFireHoseShot()
{
	ServerRPC_OnFireHoseShot();
}

void AFireMan::ServerRPC_OnFireHoseShot_Implementation()
{
	if (bDoesCarryingPerson) return;
	
	if (bDoesEquipFireHose)
	{
		// shoot water
		if (AFireHose* fireHose = Cast<AFireHose>(WaterComp->GetChildActor()))
		{
			fireHose->OnWaterShot();
		}
	}
}

void AFireMan::OnUseTool()
{
	if (!bCanUseCrowbar || !bCanUseFireHose)
	{
		OnGetCrowbar();
		OnGetFireHose();
		return;
	}

	if (bDoesEquipFireHose) return;
	
	if (bDoesEquipCrowbar)
	{
		// Check SubMission Index
		if (FireManMainUIWidget->GetCurSubMissionNum() < 2)
		{
			// Print Alert Text
			FireManMainUIWidget->ShowAlert();
			return;
		}
		// force open the door
		ServerRPC_OpenDoor();
	}
	else
	{
		// find person2
		ServerRPC_CarryPerson();
	}
}

void AFireMan::OnMaskOut()
{
	// Check SubMission Index
	if (FireManMainUIWidget->GetCurSubMissionNum() < 1)
	{
		// Print Alert Text
		FireManMainUIWidget->ShowAlert();
		return;
	}
	ServerRPC_OnMaskOut();
}

void AFireMan::ServerRPC_OnMaskOut_Implementation()
{
	Multicast_OnMaskOut();
}

void AFireMan::Multicast_OnMaskOut_Implementation()
{
	// give mask to person2
	APeopleBase* person = Cast<APeopleBase>(UGameplayStatics::GetActorOfClass(GetWorld(), APeopleBase::StaticClass()));
	if (person)
	{
		float dist = FVector::Distance(GetActorLocation(), person->GetActorLocation());
		if (dist <= InteractDist)
		{
			GetWorld()->SpawnActor<AInteractActor>(MaskActor, FTransform(GetActorLocation() + GetActorForwardVector() * MaskSpawnDist));
			
			OnMissionComplete();
		}
	}
}

void AFireMan::ServerRPC_OpenDoor_Implementation()
{
	Multicast_OpenDoor();
}

void AFireMan::Multicast_OpenDoor_Implementation()
{
	if (!DoorActor) return;
	
	float dist = FVector::Distance(GetActorLocation(), DoorActor->GetActorLocation());
	if (dist <= InteractDist)
	{
		DoorActor->ToggleWidget(false);
		DoorActor->ToggleDoor();

		if (IsLocallyControlled())
		{
			FireManMainUIWidget->SuccessSubMission();
		}
	}
}

void AFireMan::ServerRPC_CarryPerson_Implementation()
{
	Multicast_CarryPerson();
}

void AFireMan::Multicast_CarryPerson_Implementation()
{
	// if now carrying person
	if (bDoesCarryingPerson)
	{
		bDoesCarryingPerson = false;
		if (Person2Actor)
		{
			float dist = FVector::Distance(GetActorLocation(), Person2Actor->GetActorLocation());
			if (dist <= InteractDist)
			{
				// detach Person2Actor
				
				Person2Actor->ToggleWidget(true);
				Person2Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				Person2Actor->SetActorRotation(ActorRotation);
			}
		}
	}
	else
	{
		bDoesCarryingPerson = true;
		ActorRotation = Person2Actor->GetActorRotation();
		if (Person2Actor)
		{
			float dist = FVector::Distance(GetActorLocation(), Person2Actor->GetActorLocation());
			if (dist <= InteractDist)
			{
				// attach person
				Person2Actor->ToggleWidget(false);
				Person2Actor->AttachToComponent(Person2Pos, FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
		}
	}
}

void AFireMan::OffFireHose()
{
	if (!bDoesEquipFireHose) return;
		
	if (AFireHose* fireHose = Cast<AFireHose>(WaterComp->GetChildActor()))
	{
		if (fireHose->GetDoesWaterShotNow())
		{
			fireHose->OnWaterShot();
		}
	}
}

void AFireMan::OnMissionComplete()
{
	Multicast_OnMissionComplete();
}

void AFireMan::Multicast_OnMissionComplete_Implementation()
{
	if (CurrentMissionIndex > MaxMissionIndex) return;

	if (AHousePlayerState* hps = Cast<AHousePlayerState>(GetPlayerState()))
	{
		if (IsLocallyControlled())
		{
			if (CurrentMissionIndex == 1)
			{
				FireManMainUIWidget->ShowSubMissionUI(ESubMissions::FireOff);
			}
			else if (CurrentMissionIndex == 2)
			{
				FireManMainUIWidget->SuccessSubMission();
			}
		}
		
		
		CurrentMissionIndex++;
		hps->SetMissionComplete();
	}
}

void AFireMan::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;
	if (OtherActor->ActorHasTag(FName("MainLastMission")))
	{
		const FName Profile = OtherComp->GetCollisionProfileName();
		if (Profile == SafeZoneCollisionProfileName)
		{
			OnMissionComplete();
		}
	}

	// NPC 감지
	ANPCBase* NPC = Cast<ANPCBase>(OtherActor);
	if (NPC && !IsDetected)
	{
		//UE_LOG(LogTemp, Display, TEXT("OnCapsuleBeginOverlap"));
		IsDetected = true;

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]
		{
			IsDetected = false;
			
		}), 3.f, false);
	}
}

