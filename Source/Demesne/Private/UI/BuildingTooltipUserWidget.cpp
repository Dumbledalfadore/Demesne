// Created by WarRoom Wizards ©2023

#include "UI/BuildingTooltipUserWidget.h"

#include "Settlements/BuildingData.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UBuildingTooltipUserWidget::UpdateTooltip(UBuildingData* Building)
{
	BuildingNameText->SetText(FText::FromString(Building->BuildingName));
	bool DescriptionIsEmpty = true;
	bool ResourceCostIsEmpty = true;
	bool PerTurnIsEmpty = true;
	
	FString Description;
	FString ResourceCostDescription;
	FString ResourcePerTurnDescription;
	
	Description += Building->BuildingDescription;
	if(Building->BuildingDescription.Len() < 1)
	{
		DescriptionIsEmpty = true;
	}
	else
	{
		DescriptionIsEmpty = false;
	}

	/* Resource Cost to build */
	for(FResourceData& Resource : Building->ResourcesToBuild)
	{
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(Resource.Resource).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailString = "Cost to build: <Negative>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";

		/* We don't need a new line if it's the first line of the description */
		if(ResourceCostDescription.IsEmpty())
		{
			ResourceCostDescription += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}
		else /* We do want a new line otherwise */
		{
			ResourceCostDescription += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}

		ResourceCostIsEmpty = false;
	}
	
	/* Resources Produced or Consumed per turn */
	for(FResourceData& Resource : Building->ResourcesPerTurn)
	{
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(Resource.Resource).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailStringPos = "produced per turn: <Positive>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";
		FString ResourceDetailStringNeg = "consumed per turn: <Negative>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";
		FString ResourceDetailStringToUse = Resource.ResourceAmount > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;

		/* We don't need a new line if it's the first line of the description */
		if(ResourcePerTurnDescription.IsEmpty())
		{
			ResourcePerTurnDescription += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
		}
		else /* We do want a new line otherwise */
		{
			ResourcePerTurnDescription += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
		}

		PerTurnIsEmpty = false;
	}
	
	/* Removes the empty line from the end */
	Description.TrimEndInline();
	ResourceCostDescription.TrimEndInline();
	ResourcePerTurnDescription.TrimEndInline();

	DescriptionIsEmpty == true ? DescriptionLayout->SetVisibility(ESlateVisibility::Collapsed) : DescriptionLayout->SetVisibility(ESlateVisibility::Visible);
	ResourceCostIsEmpty == true ? CostLayout->SetVisibility(ESlateVisibility::Collapsed) : CostLayout->SetVisibility(ESlateVisibility::Visible);
	PerTurnIsEmpty == true ? OngoingLayout->SetVisibility(ESlateVisibility::Collapsed) : OngoingLayout->SetVisibility(ESlateVisibility::Visible);

	BuildingDescriptionText->SetText(FText::FromString(Description));
	CostDescriptionText->SetText(FText::FromString(ResourceCostDescription));
	TurnCostDescriptionText->SetText(FText::FromString(ResourcePerTurnDescription));
}
