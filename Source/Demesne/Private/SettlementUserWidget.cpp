// Created by WarRoom Wizards ©2023


#include "SettlementUserWidget.h"
#include "DemesnePlayerController.h"
#include "Settlement.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SettlementButtonUserWidget.h"
#include "UI/BuildingTooltipUserWidget.h"


void USettlementUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Controller = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if(Controller)
	{
		Controller->OnSettlementUpdated.AddDynamic(this, &USettlementUserWidget::OnSettlementUpdated);
	}

	if (BuildingTooltipClass)
	{
		/* Create the tooltip here so we aren't creating it for each button */
		BuildingTooltip = CreateWidget<UBuildingTooltipUserWidget>(this->GetGameInstance(), BuildingTooltipClass);
	}
	
	BackgroundColours.Add(EBuildingType::Settlement, FColor::FromHex("B97621FF")); /* Settlement building colour */
	BackgroundColours.Add(EBuildingType::Farming, FColor::FromHex("57552AFF")); /* Farm building colour */
	BackgroundColours.Add(EBuildingType::Military, FColor::FromHex("AF3A1BFF")); /* Military building colour */
}

void USettlementUserWidget::OnSettlementUpdated()
{
	/* Get the settlement */
	Settlement = Cast<ASettlement>(Owner);

	if(Settlement)
	{
		SettlementNameText->SetText(FText::FromString(Settlement->GetSettlementName()));
		UpdateButtons(Settlement->GetBuildingCap());

		if(BuildingOptionsOverlay)
		{
			BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USettlementUserWidget::OnNextTurn()
{
	OnSettlementUpdated();
}

FString USettlementUserWidget::GetBuildingCountAsString()
{
	return FString::FromInt(Settlement->GetBuildingCount()) + " / " + FString::FromInt(Settlement->GetBuildingCap());
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
			auto Value = UEnum::GetDisplayValueAsText(BuildingDataArray[i]->BuildingTier);
			ButtonArray[i]->SetTier(Value.ToString());
		}
		else if(ButtonArray[i] && !BuildingDataArray[i])
		{
			ButtonArray[i]->SetIcon(BuildIcon);
			ButtonArray[i]->SetBackground(FLinearColor::White);
			ButtonArray[i]->SetTier("");
		}
	}
}

void USettlementUserWidget::OnSlotButtonClicked(int Index)
{
	// UE_LOG(LogTemp, Warning, TEXT("Slot Button Clicked: %d"), Index);
	CurrentSlotIndex = Index;
	
	TArray<UBuildingData*> Temp;
	if(UBuildingData* Building = Settlement->GetBuildingAtIndex(Index))
	{
		Temp = Settlement->GetUpgradeBuildings(Building);
	}
	else
	{
		Temp = Settlement->GetBuildingsToBuild();
	}

	if(Temp.IsEmpty())
	{
		/* Fixes an issue where you could click a different slot with the menu up and build duplicated buildings */
		BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Hidden); 
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
		/* Build the selected building*/
		Settlement->BuildBuilding(Building, CurrentSlotIndex);

		/* Update the slot UI's
		 * TODO: Add function to update a single button UI
		 */
		UpdateButtonUIs(MainBuildingButtons, Settlement->GetCurrentBuildings());

		/* Hide the building overlay */
		BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Hidden);
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
			BuildingTooltip->UpdateTooltip(Building);
			BuildingTooltip->SetVisibility(ESlateVisibility::Visible);
		}
		else /* We want to hide it as we have no information to display TODO: Add information about the empty slot e.g. population required to unlock the slot */
		{
			BuildingTooltip->SetVisibility(ESlateVisibility::Hidden);
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
			BuildingTooltip->UpdateTooltip(BuildingsToBuild[Index]);
			BuildingTooltip->SetVisibility(ESlateVisibility::Visible);
		}
		else /* This code should never fire but it's here just in case */
		{
			BuildingTooltip->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
