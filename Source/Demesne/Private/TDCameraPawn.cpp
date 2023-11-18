// Created by WarRoom Wizards ©2023

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TDCameraPawn.h"

// Sets default values
ATDCameraPawn::ATDCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm->SetWorldRotation(DefaultRotation);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->TargetArmLength = SpringArmMax;
	SpringArm->bDoCollisionTest = false;

	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;

	Movement->MaxSpeed = MovementSpeed;

	/* Set these to stop the camera having a speed up / slow down */
	Movement->Acceleration = 500000.f;
	Movement->Deceleration = 500000.f;
}

// Called when the game starts or when spawned
void ATDCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATDCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	/* Get the player controller */
	APlayerController* PC = Cast<APlayerController>(GetController());

	/* Get the local player subsystem */
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	/* Clear the current mapping and add in the camera mapping */
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	/* Get the enhanced player input component */
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	/* Bind the input actions */
	PEI->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ATDCameraPawn::Move);
	PEI->BindAction(RotateInput, ETriggerEvent::Triggered, this, &ATDCameraPawn::Rotate);
	PEI->BindAction(ZoomInput, ETriggerEvent::Triggered, this, &ATDCameraPawn::Zoom);
}

void ATDCameraPawn::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		/* Move Value will range from -1 to 1 */
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);


		/* Forwards / Backwards direction */
		if (MoveValue.Y != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);

			AddMovementInput(Direction, MoveValue.Y);
		}

		/* Right / Left direction */
		if (MoveValue.X != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void ATDCameraPawn::Rotate(const FInputActionValue& Value)
{
	const float RotateValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Rotate %f"), RotateValue);

	AddControllerYawInput(RotateValue * RotateSpeed * GetWorld()->GetDeltaSeconds());
}

void ATDCameraPawn::Zoom(const FInputActionValue& Value)
{
	/* Get and invert the value, scrolling in should zoom in */
	float Amount = -Value.Get<float>();
	Amount *= ZoomSpeed;

	/* No need to continue if its outside the min/max values */
	float TargetValue = SpringArm->TargetArmLength + Amount;
	if (TargetValue > SpringArmMax || TargetValue < SpringArmMin)
	{
		return;
	}

	/* Adjust the spring arm length */
	SpringArm->TargetArmLength += Amount;
}

