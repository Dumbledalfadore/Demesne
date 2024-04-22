// Created by WarRoom Wizards ©2023
#include "TDCameraPawn.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "StrategyLayerGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Settlements/Settlement.h"
#include "Settlements/SettlementManager.h"
#include "DemesnePlayerController.h"

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
	SpringArm->TargetArmLength = SpringArmMax/ 2; /* Set to the midway point*/
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
	playerControllerRef = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM && GM->SettlementManager)
	{
		/* Set focus to the main settlement */
		auto Settlement = GM->SettlementManager->GetSettlementsByID(0)[0];
		if(Settlement)
		{
			auto SettlementPos = Settlement->GetActorLocation();
			SetLocation(SettlementPos);
		}
	}
	
}

// Called every frame
void ATDCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATDCameraPawn::Move_Implementation(const FVector2D& Value)
{
	
	if (Controller != nullptr)
	{
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);


		/* Forwards / Backwards direction */
		if (Value.Y != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			if (!Task1Complete)
			{
				playerControllerRef->CompleteTask(1);
				Task1Complete = true;
			}
			
			AddMovementInput(Direction, Value.Y);
		}

		/* Right / Left direction */
		if (Value.X != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
			if (!Task2Complete)
			{
				playerControllerRef->CompleteTask(2);
				Task2Complete = true;
			}
			AddMovementInput(Direction, Value.X);
		}
	}
}

void ATDCameraPawn::Look_Implementation(const FVector2D& Value)
{
	/*	For the camera map we only want to rotate on one axis, however the third person character may want 2-axis rotation */
	if (!Task3Complete)
	{
		playerControllerRef->CompleteTask(3);
		Task3Complete = true;
	}
	AddControllerYawInput(Value.X * RotateSpeed * GetWorld()->GetDeltaSeconds());
}

void ATDCameraPawn::Zoom_Implementation(const float& Value)
{
	/* Calculate where the zoom would be */
	float TargetValue = SpringArm->TargetArmLength + (Value * ZoomSpeed * GetWorld()->GetDeltaSeconds());
	/* No need to continue if its outside the min/max values */
	if (TargetValue > SpringArmMax || TargetValue < SpringArmMin) return;

	/* Adjust the spring arm length */
	SpringArm->TargetArmLength = TargetValue;
	if (!Task4Complete)
	{
		playerControllerRef->CompleteTask(4);
		Task4Complete = true;
	}
}

void ATDCameraPawn::SetLocation(const FVector& Location)
{
	SetActorLocation(FVector{Location.X, Location.Y, GetActorLocation().Z});
}

