// Created by WarRoom Wizards ©2023

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BasicInput.h"
#include "DemesnePlayerController.h"

// Called when the game starts or when spawned
void ADemesnePlayerController::BeginPlay()
{
	Super::BeginPlay();

	/* Add input mapping context */
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

// Called to bind functionality to input
void ADemesnePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/* Setup the action bindings */
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (EIC)
	{
		/* Movement */
		EIC->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ADemesnePlayerController::Move);

		/* Looking */
		EIC->BindAction(LookInput, ETriggerEvent::Triggered, this, &ADemesnePlayerController::Look);

		/* Zooming */
		EIC->BindAction(ZoomInput, ETriggerEvent::Triggered, this, &ADemesnePlayerController::Zoom);
	}
}

void ADemesnePlayerController::Move(const FInputActionValue& Value)
{
	/* Get the input value which is Vector2D */
	FVector2D MovementVector = Value.Get<FVector2D>();

	/* We dont want to spend time casting if the value is zero */
	if (MovementVector.IsZero()) return;

	/* Get reference to pawn so we dont have to keep calling GetPawn() */
	APawn* CurrentPawn = GetPawn();

	/* Cast to the input interface */
	IBasicInput* InputInterface = Cast<IBasicInput>(CurrentPawn);

	/* Check if the interface is valid */
	if (InputInterface)
	{
		/* Send movement data to the current pawn */
		InputInterface->Execute_Move(CurrentPawn, MovementVector);
	}
}

void ADemesnePlayerController::Look(const FInputActionValue& Value)
{
	/* Get the input value which is Vector2D */
	FVector2D LookVector = Value.Get<FVector2D>();

	/* We dont want to spend time casting if the value is zero */
	if (LookVector.IsZero()) return;

	/* Get reference to pawn so we dont have to keep calling GetPawn() */
	APawn* CurrentPawn = GetPawn();

	/* Cast to the input interface */
	IBasicInput* InputInterface = Cast<IBasicInput>(CurrentPawn);

	/* Check if the interface is valid */
	if (InputInterface)
	{
		/* Send look data to the current pawn */
		InputInterface->Execute_Look(CurrentPawn, LookVector);
	}
}

void ADemesnePlayerController::Zoom(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("WE GOT HERE: Z"));
	/* Get the input value which is a float */
	float ZoomValue = Value.Get<float>();

	/* We dont want to spend time casting if the value is zero */
	if (ZoomValue == 0) return;

	/* Get reference to pawn so we dont have to keep calling GetPawn() */
	APawn* CurrentPawn = GetPawn();

	/* Cast to the input interface */
	IBasicInput* InputInterface = Cast<IBasicInput>(CurrentPawn);

	/* Check if the interface is valid */
	if (InputInterface)
	{
		/* Send look data to the current pawn */
		InputInterface->Execute_Zoom(CurrentPawn, ZoomValue);
	}
}
