// Created by WarRoom Wizards ©2023

#include "UI/BuildingTooltipUserWidget.h"

#include "BuildingData.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

void UBuildingTooltipUserWidget::UpdateTooltip(UBuildingData* Building)
{
	BuildingNameText->SetText(FText::FromString(Building->BuildingName));
	bool IsEmpty = true;
	
	FString Description;
	/* Resources Produced or Consumed */
	for(FResourceData& Resource : Building->Resources)
	{
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(Resource.Resource).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailStringPos = "produced per turn: <Positive>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";
		FString ResourceDetailStringNeg = "consumed per turn: <Negative>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";
		FString ResourceDetailStringToUse = Resource.ResourceAmount > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;

		/* We don't need a new line if it's the first line of the description */
		if(Description.IsEmpty())
		{
			Description += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
		}
		else /* We do want a new line otherwise */
		{
			Description += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
		}

		IsEmpty = false;
	}
	
	/* Removes the empty line from the end */
	Description.TrimEndInline();

	IsEmpty == true ? BuildingDescriptionText->SetVisibility(ESlateVisibility::Collapsed) : BuildingDescriptionText->SetVisibility(ESlateVisibility::Visible);

	BuildingDescriptionText->SetText(FText::FromString(Description));
}
