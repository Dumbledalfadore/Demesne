// Created by WarRoom Wizards ©2023


#include "UI/SettlementUserWidget.h"
#include "DemesnePlayerController.h"
#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "Settlements/Settlement.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SettlementButtonUserWidget.h"
#include "UI/BuildingTooltipUserWidget.h"


void USettlementUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Controller = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	SettlementButton->OnClicked.AddUniqueDynamic(this, &ThisClass::USettlementUserWidget::SwitchToSettlement);
	GarrisonButton->OnClicked.AddUniqueDynamic(this, &ThisClass::USettlementUserWidget::SwitchToGarrison);

	if(Controller)
	{
		Controller->OnSettlementUpdated.AddDynamic(this, &USettlementUserWidget::OnSettlementUpdated);
	}

	/* Bind turn manager delegate */
	if(GM && GM->EconComp && GM->EconComp->TurnManagerRef)
	{
		GM->EconComp->TurnManagerRef->OnTurnEnd.AddUniqueDynamic(this, &ThisClass::OnSettlementUpdated);
	}

	if (BuildingTooltipClass)
	{
		/* Create the tooltip here so we aren't creating it for each button */
		BuildingTooltip = CreateWidget<UBuildingTooltipUserWidget>(this->GetGameInstance(), BuildingTooltipClass);
	}

	AddBackgroundColour(EBuildingType::Misc, FColor::White); /* Misc building colour */
	AddBackgroundColour(EBuildingType::Settlement, FColor::FromHex("B97621FF")); /* Settlement building colour */
	AddBackgroundColour(EBuildingType::Farming, FColor::FromHex("57552AFF")); /* Farm building colour */
	AddBackgroundColour(EBuildingType::Military, FColor::FromHex("AF3A1BFF")); /* Military building colour */
	AddBackgroundColour(EBuildingType::Fishing, FColor::FromHex("2EA5FFFF")); /* Fishing building colour */
	AddBackgroundColour(EBuildingType::Craft, FColor::FromHex("650473FF")); /* Craft building colour */

	NotificationOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void USettlementUserWidget::OnSettlementUpdated()
{
	/* If we have an old settlement, unbind from the notification delegate */
	if(Settlement)
	{
		Settlement->OnNotification.RemoveDynamic(this, &ThisClass::OnNotification);
	}
	
	/* Get the settlement */
	Settlement = Cast<ASettlement>(Owner);

	if(Settlement)
	{
		/* Bind to the new settlement notificaiton delegate */
		Settlement->OnNotification.AddUniqueDynamic(this, &ThisClass::OnNotification);
		SettlementNameText->SetText(FText::FromString(Settlement->GetSettlementName()));
		PopulationText->SetText(FText::FromString(FString::FromInt(Settlement->GetPopulation())));
		UpdateButtons(Settlement->GetBuildingCap());

		UpdateResourceValues();

		if(BuildingOptionsOverlay)
		{
			BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USettlementUserWidget::OnNextTurn()
{
	OnSettlementUpdated();
}

void USettlementUserWidget::AddBackgroundColour(EBuildingType Type, FLinearColor Colour)
{
	/* Write over existing data*/
	if(BackgroundColours.Contains(Type))
	{
		BackgroundColours.Emplace(Type, Colour);
	}
	else /* Add new data */
	{
		BackgroundColours.Add(Type, Colour);
	}
}

FLinearColor USettlementUserWidget::GetBackgroundColour(EBuildingType Type)
{
	return BackgroundColours.FindRef(Type);
}

void USettlementUserWidget::CreateButtons(TArray<USettlementButtonUserWidget*>& ButtonArray, UWrapBox* WrapBox, int Count, bool IsBaseSlot)
{
	/* Make sure we delete any buttons that already exist first */
	if(ButtonArray.Num() > 0)
	{
		for(auto& Button : ButtonArray)
		{
			/* Unbind delegates */
			if(IsBaseSlot)
			{
				Button->OnClickedDelegate.RemoveDynamic(this, &ThisClass::OnSlotButtonClicked);
				Button->OnHoveredDelegate.RemoveDynamic(this, &ThisClass::OnSlotButtonHovered);
			}
			else
			{
				Button->OnClickedDelegate.RemoveDynamic(this, &ThisClass::OnBuildButtonClicked);
				Button->OnHoveredDelegate.RemoveDynamic(this, &ThisClass::OnBuildButtonHovered);
			}
			

			/* Remove button */
			Button->RemoveFromParent();
		}

		/* Empty array just to be sure */
		ButtonArray.Empty();
	}

	/* Create the buttons */
	for (int i = 0; i <= Count - 1; i++)
	{
		USettlementButtonUserWidget* Button = Cast<USettlementButtonUserWidget>(CreateWidget(Controller, BuildingSlotButtonClass));
		if(Button && WrapBox)
		{
			/* Set the index for use when building or upgrading buildings etc. */
			Button->Index = i;
			Button->SetToolTip(BuildingTooltip);

			/* Create delegates for the buttons so we can get the index */
			if(IsBaseSlot)
			{
				Button->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::OnSlotButtonClicked);
				Button->OnHoveredDelegate.AddUniqueDynamic(this, &ThisClass::OnSlotButtonHovered);
			}
			else
			{
				Button->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::OnBuildButtonClicked);
				Button->OnHoveredDelegate.AddUniqueDynamic(this, &ThisClass::OnBuildButtonHovered);
			}

			/* Add the button to the screen */
			WrapBox->AddChild(Button);

			/* Add the button to the array */
			ButtonArray.Add(Button);
		}
	}
}

void USettlementUserWidget::UpdateButtons(int Count)
{
	/* Create the main buttons */
	if(Settlement)
	{
		CreateButtons(MainBuildingButtons, CurrentBuildingWrapBox, Settlement->GetBuildingCap(), true);
		UpdateButtonUIs(MainBuildingButtons, Settlement->GetCurrentBuildings());
	}
}

void USettlementUserWidget::UpdateButtonUIs(TArray<USettlementButtonUserWidget*> ButtonArray,
	TArray<UBuildingData*> BuildingDataArray)
{
	for(int i = 0; i <= ButtonArray.Num() - 1; i++)
	{
		if(ButtonArray[i] && BuildingDataArray[i])
		{
			ButtonArray[i]->SetIcon(BuildingDataArray[i]->Icon);
			FLinearColor TempColour = GetBackgroundColour(BuildingDataArray[i]->BuildingType);
			ButtonArray[i]->SetBackground(TempColour);
			FText Value = UEnum::GetDisplayValueAsText(BuildingDataArray[i]->BuildingTier);
			bool PopulationIsLower = Settlement->GetPopulation() < Settlement->GetLocalResourceValue(BuildingDataArray[i], ELocalResourceType::Population);
			ButtonArray[i]->SetPopulationVisible( PopulationIsLower ? ESlateVisibility::Visible : ESlateVisibility::Collapsed );
			ButtonArray[i]->SetButtonEnabled(!PopulationIsLower);
			ButtonArray[i]->SetPopulation(FString::FromInt(Settlement->GetLocalResourceValue(BuildingDataArray[i], ELocalResourceType::Population)));
			ButtonArray[i]->SetIsLocked(false);
						
			if(BuildingDataArray[i]->BuildingType == EBuildingType::Misc)
			{
				ButtonArray[i]->SetTier("");
				ButtonArray[i]->SetPopulationVisible(ESlateVisibility::Collapsed);
				if(BuildingDataArray[i]->BuildingName == "Building Expansion" && i > Settlement->GetBuildingCapAvailable() - 1)
				{
					ButtonArray[i]->SetButtonEnabled(false);
					//ButtonArray[i]->SetToolTip(nullptr);
					ButtonArray[i]->SetIsLocked(true);
				}
				else
				{
					ButtonArray[i]->SetToolTip(BuildingTooltip);
					ButtonArray[i]->SetButtonEnabled(true);
				}
			}
			else
			{
				ButtonArray[i]->SetTier(Value.ToString());
			}
		}
		else if(ButtonArray[i] && !BuildingDataArray[i])
		{
			ButtonArray[i]->SetIcon(BuildIcon);
			ButtonArray[i]->SetBackground(FLinearColor::White);
			ButtonArray[i]->SetTier("");
		}
	}
}

void USettlementUserWidget::UpdateResourceValues()
{
	FoodText->SetText(FText::FromString(FString::FromInt(Settlement->GetLocalFoodIncome())));
	GoldText->SetText(FText::FromString(FString::FromInt(Settlement->GetLocalGoldIncome())));
}

void USettlementUserWidget::OnNotification(FString Notification)
{
	/* Clear any old timer */
	GetWorld()->GetTimerManager().ClearTimer(NotificationTimerHandle);

	/* Set the UI active*/
	NotificationOverlay->SetVisibility(ESlateVisibility::Visible);
	NotificationText->SetText(FText::FromString(Notification));

	/* Start new timer */
	GetWorld()->GetTimerManager().SetTimer(NotificationTimerHandle, this, &ThisClass::OnNotificationStop, NotificationTimer, false);
}

void USettlementUserWidget::OnNotificationStop()
{
	NotificationOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void USettlementUserWidget::SwitchToSettlement()
{
	SettlementWidgetSwitcher->SetActiveWidgetIndex(0);
}

void USettlementUserWidget::SwitchToGarrison()
{
	SettlementWidgetSwitcher->SetActiveWidgetIndex(1);
}

void USettlementUserWidget::OnSlotButtonClicked(int Index)
{
	// UE_LOG(LogTemp, Warning, TEXT("Slot Button Clicked: %d"), Index);
	CurrentSlotIndex = Index;
	
	TArray<UBuildingData*> Temp;
	/* If a building exists, the we want to find upgrades */
	if(UBuildingData* Building = Settlement->GetBuildingAtIndex(Index))
	{
		Temp = Settlement->GetUpgradeBuildings(Building);
	}
	else /* else we want any buildings we can build */
	{
		Temp = Settlement->GetBuildingsToBuild();
	}

	if(Temp.IsEmpty())
	{
		/* Fixes an issue where you could click a different slot with the menu up and build duplicated buildings */
		BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Collapsed); 
		return;
	}
	BuildingsToBuild = Temp;
	
	BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Visible);
	CreateButtons(SubBuildingButtons, BuildingOptionsWrapBox, Temp.Num(), false);
	UpdateButtonUIs(SubBuildingButtons, Temp);
}

void USettlementUserWidget::OnBuildButtonClicked(int Index)
{
	// UE_LOG(LogTemp, Warning, TEXT("Build Button Clicked: %d"), Index);

	if(UBuildingData* Building = BuildingsToBuild[Index])
	{
		/* We want to deconstruct the building rather than build one */
		if(Building->BuildingName == "Deconstruct Building")
		{
			Settlement->BuildBuilding(Settlement->GetEmptyBuilding(), CurrentSlotIndex);
		}
		else
		{
			/* Build the selected building*/
			Settlement->BuildBuilding(Building, CurrentSlotIndex);
		}
		
		/* We may have built or upgraded resource producing building - update UI */
		UpdateResourceValues();

		/* Update the slot UI's
		 * TODO: Add function to update a single button UI
		 */
		UpdateButtonUIs(MainBuildingButtons, Settlement->GetCurrentBuildings());

		/* Hide the building overlay */
		BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USettlementUserWidget::OnSlotButtonHovered(int Index)
{
	if(!Settlement)
	{
		return;
	}
	
	if(BuildingTooltip)
	{
		if(UBuildingData* Building = Settlement->GetCurrentBuildings()[Index])
		{
			BuildingTooltip->UpdateTooltip(Building, true);
			BuildingTooltip->SetVisibility(ESlateVisibility::Visible);
		}
		else /* We want to hide it as we have no information to display */
		{
			BuildingTooltip->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USettlementUserWidget::OnBuildButtonHovered(int Index)
{
	if(!Settlement)
	{
		return;
	}
	
	if(BuildingTooltip)
	{
		if(BuildingsToBuild[Index])
		{
			BuildingTooltip->UpdateTooltip(BuildingsToBuild[Index], false);
			BuildingTooltip->SetVisibility(ESlateVisibility::Visible);
		}
		else /* This code should never fire but it's here just in case */
		{
			BuildingTooltip->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
