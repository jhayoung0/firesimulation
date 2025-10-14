// Fill out your copyright notice in the Description page of Project Settings.


#include "FireMan.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"


// Sets default values
AFireMan::AFireMan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> firemanMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/CHARACTERS/PLAYER/B-01_TACTICAL/fix_v2/SKM_B-01_v4_BRAWNY_SIMPLE.SKM_B-01_v4_BRAWNY_SIMPLE'"));
	if (firemanMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(firemanMeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));

	FiremanCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FiremanCamera"));
	FiremanCamera->SetupAttachment(GetMesh(), TEXT("head"));
	FiremanCamera->SetRelativeLocationAndRotation(FVector(31.610969,0.863677,0), FRotator(0,20,-90));

	FirehosePos = CreateDefaultSubobject<USceneComponent>(TEXT("FirehosePos"));
	FirehosePos->SetupAttachment(RootComponent);
	FirehosePos->SetRelativeLocation(FVector(90.000000,20.000000,60.000000));

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
	ConstructorHelpers::FObjectFinder<UInputAction> interactActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Interact.IA_Interact'"));
	if (interactActionRef.Succeeded())
	{
		InteractAction = interactActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> AxeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/Fireman/IA_Axe.IA_Axe'"));
	if (AxeActionRef.Succeeded())
	{
		AxeAction = AxeActionRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> fireHoseActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/Fireman/IA_FireHose.IA_FireHose'"));
	if (fireHoseActionRef.Succeeded())
	{
		FireHoseAction = fireHoseActionRef.Object;
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
		playerInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AFireMan::OnInteract);
		playerInput->BindAction(AxeAction, ETriggerEvent::Started, this, &AFireMan::OnAxe);
		playerInput->BindAction(FireHoseAction, ETriggerEvent::Started, this, &AFireMan::OnFireHose);
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
}

void AFireMan::OnInteract(const struct FInputActionValue& value)
{
}

void AFireMan::OnAxe(const struct FInputActionValue& value)
{
}

void AFireMan::OnFireHose(const struct FInputActionValue& value)
{
}

