// Created by WarRoom Wizards ©2023

#include "UI/BuildingTooltipUserWidget.h"

#include "BuildingData.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

void UBuildingTooltipUserWidget::UpdateTooltip(UBuildingData* Building)
{
	BuildingNameText->SetText(FText::FromString(Building->BuildingName));

	FString Description;
	/* Resources Produced */
	for(FResourceData& Resource : Building->ResourcesProduced)
	{
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(Resource.Resource).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailString = "generated per turn: <Positive>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";

		/* We don't need a new line if it's the first line of the description */
		if(Description.IsEmpty())
		{
			Description += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}
		else /* We do want a new line otherwise */
		{
			Description += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;;
		}
	}

	/* Resources Consumed */
	for(FResourceData& Resource : Building->ResourceUpkeep)
	{
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(Resource.Resource).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailString = "consumed per turn: <Negative>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";

		/* We don't need a new line if it's the first line of the description */
		if(Description.IsEmpty())
		{
			Description += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}
		else /* We do want a new line otherwise */
		{
			Description += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;;
		}
	}

	/* Removes the empty line from the end */
	Description.TrimEndInline();

	BuildingDescriptionText->SetText(FText::FromString(Description));
}
