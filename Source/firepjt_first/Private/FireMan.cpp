// Fill out your copyright notice in the Description page of Project Settings.


#include "FireMan.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FireHose.h"
#include "FiremanAnim.h"
#include "InteractActor.h"
#include "PeopleBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Kismet/GameplayStatics.h"


class AInteractActor;
// Sets default values
AFireMan::AFireMan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Fireman Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> firemanMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/CHARACTERS/PLAYER/B-01_TACTICAL/fix_v2/SKM_B-01_v4_BRAWNY_SIMPLE.SKM_B-01_v4_BRAWNY_SIMPLE'"));
	if (firemanMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(firemanMeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));

	// Fireman Camera
	FiremanCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FiremanCamera"));
	FiremanCamera->SetupAttachment(GetMesh(), TEXT("head"));
	FiremanCamera->SetRelativeLocationAndRotation(FVector(31.610969,0.863677,0), FRotator(0,20,-90));
	FiremanCamera->bUsePawnControlRotation = true;

	// Firehose Attach Position
	FirehosePos = CreateDefaultSubobject<USceneComponent>(TEXT("FirehosePos"));
	FirehosePos->SetupAttachment(GetMesh(), TEXT("cc_weaponbone_r"));
	FirehosePos->SetRelativeLocationAndRotation(FVector(-6.5,-10.5,1), FRotator(0,-75,-90));

	// Water Niagara System
	WaterComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("WaterComp"));
	ConstructorHelpers::FClassFinder<AFireHose> WaterRef(TEXT("/Game/CustomContents/Fireman/Water/BP_FireHose.BP_FireHose_C"));
	if (WaterRef.Succeeded())
	{
		WaterComp->SetChildActorClass(WaterRef.Class);
		WaterComp->SetupAttachment(FirehosePos);
		WaterComp->SetVisibility(false);
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

	// Door Class
	ConstructorHelpers::FClassFinder<AInteractActor> doorClassRef(TEXT("/Game/CustomContents/People/Blueprints/BP_Door.BP_Door_C"));
	if (doorClassRef.Succeeded())
	{
		DoorClass = doorClassRef.Class;
	}
	
	// Mask Actor
	ConstructorHelpers::FClassFinder<AInteractActor> maskRef(TEXT("/Game/CustomContents/People/Blueprints/BP_Mask.BP_Mask_C"));
	if (maskRef.Succeeded())
	{
		MaskActor = maskRef.Class;
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
	DoorActor = UGameplayStatics::GetActorOfClass(GetWorld(), DoorClass);
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
	AddControllerPitchInput(pitch);
	FiremanAnimInstance->AddPitchInputToSpine(pitch);
}

void AFireMan::OnEquipFireHose()
{
	if (bDoesCarryingPerson) return;

	if (bDoesEquipFireHose)
	{
		// off
		OffFireHose();
		bDoesEquipFireHose = false;
		WaterComp->SetVisibility(false);
		FiremanCamera->bUsePawnControlRotation = true;
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
		FiremanCamera->bUsePawnControlRotation = false;
	}
}

void AFireMan::OnEquipCrowbar()
{
	if (bDoesCarryingPerson) return;
	
	if (bDoesEquipCrowbar)
	{
		// off
		bDoesEquipCrowbar = false;
		CrowbarMeshComp->SetVisibility(false);
		FiremanCamera->bUsePawnControlRotation = true;
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
		FiremanCamera->bUsePawnControlRotation = false;
	}
}

void AFireMan::OnFireHoseShot()
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
	if (bDoesCarryingPerson) return;

	if (bDoesEquipCrowbar)
	{
		// force open the door
		if (AInteractActor* door = Cast<AInteractActor>(DoorActor))
		{
			float dist = FVector::Distance(GetActorLocation(), door->GetActorLocation());
			if (dist <= InteractDist)
			{
				door->ToggleDoor(true);
			}
		}
	}
	// else
	// {
	// 	if (AInteractActor* door = Cast<AInteractActor>(DoorActor))
	// 	{
	// 		float dist = FVector::Distance(GetActorLocation(), door->GetActorLocation());
	// 		if (dist <= InteractDist)
	// 		{
	// 			door->ToggleDoor(true);
	// 		}
	// 	}
	// }
}

void AFireMan::OnMaskOut()
{
	// 서버 상으로 상대 편의 액터를 가져올 수 있으면 그렇게 해도 됨
	// if can get actor from other pc(server), you can get other actor that way
	APeopleBase* person = Cast<APeopleBase>(UGameplayStatics::GetActorOfClass(GetWorld(), APeopleBase::StaticClass()));
	if (person)
	{
		float dist = FVector::Distance(GetActorLocation(), person->GetActorLocation());
		if (dist <= InteractDist)
		{
			GetWorld()->SpawnActor<AInteractActor>(MaskActor, FTransform(GetActorLocation() + GetActorForwardVector() * MaskSpawnDist));
		}
	}
}

void AFireMan::OnCarryPerson()
{
	// if now equip firehose or crowbar
	if (bDoesEquipFireHose)
	{
		bDoesEquipFireHose = false;
	}
	else if (bDoesEquipCrowbar)
	{
		bDoesEquipCrowbar = false;
	}

	// if now carrying person
	if (bDoesCarryingPerson)
	{
		bDoesCarryingPerson = false;
	}
	else
	{
		bDoesCarryingPerson = true;
	}
}

void AFireMan::OffFireHose()
{
	if (!bDoesEquipFireHose) return;
		
	if (AFireHose* fireHose = Cast<AFireHose>(WaterComp->GetChildActor()))
	{
		if (fireHose->GetDoesWaterShotNow())
		{
			UE_LOG(LogTemp, Warning, TEXT("Off WaterShot"));
			fireHose->OnWaterShot();
		}
	}
}