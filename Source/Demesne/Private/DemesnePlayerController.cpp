// Created by WarRoom Wizards ©2023

#include "DemesnePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BasicInput.h"
#include "GridManager.h"
#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "Settlements/Settlement.h"
#include "UI/SettlementUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyUnit.h"
#include "UI/ArmyUserWidget.h"
#include "Utils/Pathfinder.h"
#include "Tile.h"
#include "Military/ArmyDataComponent.h"
#include "Military/ArmyManager.h"
#include "UI/BattleReportUserWidget.h"
#include "UI/OverviewPanelWidget.h"

ADemesnePlayerController::ADemesnePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bShowMenu = true;
}

// Called when the game starts or when spawned
void ADemesnePlayerController::BeginPlay()
{
	Super::BeginPlay();

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

	if(ArmyUIClass)
	{
		/* Create the army UI here so we aren't creating it for each army */
		ArmyUI = CreateWidget<UUserWidget>(this->GetGameInstance(), ArmyUIClass);

		if (ArmyUI)
		{
			ArmyUI->AddToViewport();

			/* IF the army UI was created, set it as hidden as we don't need it yet */
			ArmyUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (BattleReportUIClass)
	{
		/* Create the battle report UI here so we don't create it each battle */
		BattleReportUI = CreateWidget<UUserWidget>(this->GetGameInstance(), BattleReportUIClass);

		if (BattleReportUI)
		{
			BattleReportUI->AddToViewport();

			/* IF the battle report UI was created, set it as hidden as we don't need it yet */
			BattleReportUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if(OverviewUIClass)
	{
		/* Create the UI */
		OverviewUI = CreateWidget<UUserWidget>(this->GetGameInstance(), OverviewUIClass);

		if(OverviewUI)
		{
			OverviewUI->AddToViewport();
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

	if(PauseUIClass)
	{
		/* Create the UI */
		PauseUI = CreateWidget<UUserWidget>(this->GetGameInstance(), PauseUIClass);
	}

	/* Add input mapping context */
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if(GM && GM->ArmyManager)
	{
		GM->ArmyManager->OnBattleReported.AddUniqueDynamic(this, &ThisClass::UpdateBattleReportUI);
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

		/* Left Click */
		EIC->BindAction(RightlickInput, ETriggerEvent::Triggered, this, &ADemesnePlayerController::OnRightClick);
		/*Menu*/
		EIC->BindAction(MenuInput, ETriggerEvent::Started, this, &ADemesnePlayerController::MenuToggle);
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

		AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if(!GM) return; 
		
		if (ASettlement* Settlement = Cast<ASettlement>(Result))
		{
			//UE_LOG(LogTemp, Warning, TEXT("SETTLEMENT, ID: %i"), Settlement->GetPlayerID());
			/* We don't need to check anything - just update the UI with new data, even if its */
			UpdateSettlementUI(Settlement);
			SetArmyUIVisibility(false);

			CurrentSelectedUnit = nullptr;
			GM->PathFinder->UnitDeselected();
		}
		else if (AArmyUnit* Army = Cast<AArmyUnit>(Result))
		{
			//UE_LOG(LogTemp, Warning, TEXT("ARMY"));
			UpdateArmyUI(Army);
			SetSettlementUIVisibility(false);

			CurrentSelectedUnit = Army;

			if(CurrentSelectedUnit->GetRemainingMovementDistance() <= 0)
			{
				GM->PathFinder->UnitDeselected();
				return;
			}
			
			GM->PathFinder->FindValidTiles(GM->GridManager->GetTile(CurrentSelectedUnit->GetTileIndex()), CurrentSelectedUnit->GetRemainingMovementDistance());
			// for(auto Neighbour : GM->GridManager->GetTile(CurrentSelectedUnit->GetTileIndex())->GetNeighbours())
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("DRAWING NEIGHBOUR: X:%i, Y:%i"), Neighbour->TileIndex.X, Neighbour->TileIndex.Y);
			// 	DrawDebugBox(GetWorld(), Neighbour->GetActorLocation() + FVector{0, 0, 90}, FVector{5, 5, 5}, FColor::Red, true);
			// }
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("NONE"));
			/* If we click anywhere else we want to hide the UI */
			SetSettlementUIVisibility(false);
			SetArmyUIVisibility(false);

			CurrentSelectedUnit = nullptr;
			GM->PathFinder->UnitDeselected();
		}
	}
}

void ADemesnePlayerController::OnRightClick(const FInputActionValue& Value)
{
	if(!CurrentSelectedUnit) return;
	
	FHitResult Hit;
	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	GetHitResultUnderCursorByChannel(TraceChannel, true, Hit);

	if (Hit.bBlockingHit)
	{
		AActor* Result = Hit.GetActor();

		AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if(!GM) return;

		if(const ATile* Tile = Cast<class ATile>(Result))
		{
			if(Tile->GetValidMoveTile() && !Tile->GetIsOccupied())
			{
				//UE_LOG(LogTemp, Warning, TEXT("VALID MOVE TILE"));
				CurrentSelectedUnit->SetActorLocation(Tile->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f));
				CurrentSelectedUnit->SetTileIndex(Tile->TileIndex);
				
				CurrentSelectedUnit->SetRemainingMovementDistance(CurrentSelectedUnit->GetRemainingMovementDistance() - Tile->GetLocalGoal());
				
				if(CurrentSelectedUnit->GetRemainingMovementDistance() <= 0)
				{
					if(GM->PathFinder)
					{
						GM->PathFinder->UnitDeselected();
					}
					return;
				}
				
				GM->PathFinder->FindValidTiles(GM->GridManager->GetTile(CurrentSelectedUnit->GetTileIndex()), CurrentSelectedUnit->GetRemainingMovementDistance());
				return;
			}

			//UE_LOG(LogTemp, Warning, TEXT("INVALID MOVE TILE"));
		}
		else if (const ASettlement* Settlement = Cast<ASettlement>(Result))
		{
			/* We don't want to attack ourselves */
			if(Settlement->GetPlayerID() == 0) return;
			/* We only want to be able to attack if we can move to the tile */
			if(const auto SettlementTile = GM->GridManager->GetTile(Settlement->GetTileIndex()))
			{
				if(!SettlementTile->GetValidMoveTile()) return;
			}
			
			//UE_LOG(LogTemp, Warning, TEXT("SETTLEMENT"));
			if(GM->ArmyManager)
			{
				UE_LOG(LogTemp, Warning, TEXT("START BATTLE AGAINST SETTLEMENT"));
				GM->ArmyManager->InitiateBattle(CurrentSelectedUnit->GetArmyComponent(), Settlement->GetGarrison());
			}
		}
		else if (const AArmyUnit* Army = Cast<AArmyUnit>(Result))
		{
			/* We don't want to attack ourselves */
			if(Army->GetPlayerID() == 0) return;
			/* We only want to be able to attack if we can move to the tile */
			if(const auto ArmyTile = GM->GridManager->GetTile(Army->GetTileIndex()))
			{
				if(!ArmyTile->GetValidMoveTile()) return;
			}
			//UE_LOG(LogTemp, Warning, TEXT("ARMY"));
			
			if(GM->ArmyManager)
			{
				UE_LOG(LogTemp, Warning, TEXT("START BATTLE AGAINST ARMY"));
				GM->ArmyManager->InitiateBattle(CurrentSelectedUnit->GetArmyComponent(), Army->GetArmyComponent());
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("NONE"));
		}
	}
}

void ADemesnePlayerController::MenuToggle(const FInputActionValue& Value)
{
	if(bShowMenu)
	{
		if(PauseUI)
		{
			PauseUI->AddToViewport();
			bShowMenu = false;
		}
	}
	else
	{
		if(PauseUI)
		{
			PauseUI->RemoveFromParent();
			bShowMenu = true;
		}
	}
}

void ADemesnePlayerController::UpdateSettlementUI(ASettlement* Settlement)
{
	//UE_LOG(LogTemp, Warning, TEXT("Settlement Clicked!"));

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
			SettlementUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ADemesnePlayerController::UpdateArmyUI(AArmyUnit* Army)
{
	if (ArmyUI)
	{
		/* Cast so we can set owner */
		auto UI = Cast<UArmyUserWidget>(ArmyUI);

		/* We want to set the owner of the UI to the current army */
		if (UI)
		{
			UI->Owner = Army;
		}

		/* Call delegate so the UI knows to update */
		OnArmyUpdated.Broadcast();

		/* We want to always set the ui visibility to true when we click on the army
		   But we want to do it after updating all the UI data */
		SetArmyUIVisibility(true);

	}
}

void ADemesnePlayerController::SetArmyUIVisibility(bool Visible)
{
	if (ArmyUI)
	{
		/* Update the UI visibility*/
		if (Visible)
		{
			ArmyUI->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ArmyUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ADemesnePlayerController::UpdateBattleReportUI(UArmyDataComponent* Aggressor, UArmyDataComponent* Defender, FString Victor)
{
	/* We only need to update the UI if the player was involved */
	if(Aggressor->GetPlayerID() != 0 && Defender->GetPlayerID() != 0) return;
	
	if (BattleReportUI)
	{
		/* Cast so we can set owner */
		auto UI = Cast<UBattleReportUserWidget>(BattleReportUI);

		/* We want to set the owner of the UI to the current army */
		if (UI)
		{
			UI->OnBattleReported(Aggressor, Defender, Victor);
		}

		/* We want to always set the ui visibility to true when we have been involved in a battle
		   But we want to do it after updating all the UI data */
		SetBattleReportUIVisibility(true);

	}
}

void ADemesnePlayerController::SetBattleReportUIVisibility(bool Visible)
{
	if (BattleReportUI)
	{
		/* Update the UI visibility*/
		if (Visible)
		{
			BattleReportUI->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			BattleReportUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ADemesnePlayerController::UpdateOverviewUI()
{
	if (OverviewUI)
	{
		/* Cast so we can set owner */
		auto UI = Cast<UOverviewPanelWidget>(OverviewUI);
		
		if (UI)
		{
			UI->RefreshSettlementUI();
		}
	}
}
