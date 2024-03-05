// Created by WarRoom Wizards ©2023

#include "DemesnePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BasicInput.h"
#include "TurnManager.h"
#include "Settlements/Settlement.h"
#include "UI/SettlementUserWidget.h"
#include "Kismet/GameplayStatics.h"

ADemesnePlayerController::ADemesnePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

// Called when the game starts or when spawned
void ADemesnePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(TurnManagerClass)
	{
		FActorSpawnParameters SpawnInfo;
		FVector Location = FVector::ZeroVector;
		FRotator Rotation = FRotator::ZeroRotator;
		ATurnManager* TurnManager = GetWorld()->SpawnActor<ATurnManager>(TurnManagerClass,Location,Rotation,SpawnInfo);
	}
	if (SettlementUIClass)
	{
		/* Create the settlement UI here so we aren't creating it for each settlement */
		SettlementUI = CreateWidget<UUserWidget>(this->GetGameInstance(), SettlementUIClass);

		if (SettlementUI)
		{
			SettlementUI->AddToViewport();

			/* IF the settlemnt UI was created, set it as hidden as we don't need it yet */
			SettlementUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(StrategyUIClass)
	{
		/* Create the UI */
		StrategyUI = CreateWidget<UUserWidget>(this->GetGameInstance(), StrategyUIClass);

		if(StrategyUI)
		{
			StrategyUI->AddToViewport();
		}
	}

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

		/* Left Click */
		EIC->BindAction(LeftClickInput, ETriggerEvent::Triggered, this, &ADemesnePlayerController::OnLeftClick);
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

void ADemesnePlayerController::OnLeftClick(const FInputActionValue& Value)
{
	FHitResult Hit;
	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	GetHitResultUnderCursorByChannel(TraceChannel, true, Hit);

	if (Hit.bBlockingHit)
	{
		AActor* Result = Hit.GetActor();
		
		ASettlement* Settlement = Cast<ASettlement>(Result);
		if (Settlement)
		{
			/* We don't need to check anything - just update the UI with new data, even if its */
			UpdateSettlementUI(Settlement);
		}
		else
		{
			/* If we click anywhere else we want to hide the UI */
			SetSettlementUIVisibility(false);
		}
	}
}

void ADemesnePlayerController::UpdateSettlementUI(ASettlement* Settlement)
{
	UE_LOG(LogTemp, Warning, TEXT("Settlement Clicked!"));

	if (SettlementUI)
	{
		/* Cast so we can set owner */
		auto UI = Cast<USettlementUserWidget>(SettlementUI);

		/* We want to set the owner of the UI to the current settlement */
		if (UI)
		{
			UI->Owner = Settlement;
		}

		/* Call delegate so the UI knows to update */
		OnSettlementUpdated.Broadcast();

		/* We want to always set the ui visibility to true when we click on the settlement
		   But we want to do it after updating all the UI data */
		SetSettlementUIVisibility(true);

	}
}

void ADemesnePlayerController::SetSettlementUIVisibility(bool Visible)
{
	if (SettlementUI)
	{
		/* Update the UI visibility*/
		if (Visible)
		{
			SettlementUI->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SettlementUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
