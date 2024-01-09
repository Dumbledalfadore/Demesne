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


void USettlementUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Controller = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if(Controller)
	{
		Controller->OnSettlementUpdated.AddDynamic(this, &USettlementUserWidget::OnSettlementUpdated);
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
			/* Unbind delegate */
			if(IsBaseSlot)
			{
				Button->OnClickedDelegate.RemoveDynamic(this, &ThisClass::OnSlotButtonClicked);
			}
			else
			{
				Button->OnClickedDelegate.RemoveDynamic(this, &ThisClass::OnBuildButtonClicked);
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

			/* Create delegate for when the button is clicked so we can get the index */
			if(IsBaseSlot)
			{
				Button->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::OnSlotButtonClicked);
			}
			else
			{
				Button->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::OnBuildButtonClicked);
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
	UE_LOG(LogTemp, Warning, TEXT("Buttons Created!"));

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
	UE_LOG(LogTemp, Warning, TEXT("Slot Button Clicked: %d"), Index);
	CurrentSlotIndex = Index;
	
	TArray<UBuildingData*> Temp;
	if(UBuildingData* Building = Settlement->GetBuildingAtIndex(Index))
	{
		Temp = Settlement->GetBuildingsByTypeAndTier(Building->BuildingType, Settlement->GetNextBuildingTier(Building));

	}
	else
	{
		Temp = Settlement->GetBuildingsToBuild();
	}

	if(Temp.IsEmpty()) return;
	BuildingsToBuild = Temp;
	
	BuildingOptionsOverlay->SetVisibility(ESlateVisibility::Visible);
	CreateButtons(SubBuildingButtons, BuildingOptionsWrapBox, Temp.Num(), false);
	UpdateButtonUIs(SubBuildingButtons, Temp);
}

void USettlementUserWidget::OnBuildButtonClicked(int Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Build Button Clicked: %d"), Index);

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
