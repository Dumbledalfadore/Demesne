// Created by WarRoom Wizards ©2023

#include "UI/BuildingTooltipUserWidget.h"

#include "Settlements/BuildingData.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Military/ArmyUnit.h"
#include "Military/UnitData.h"

void UBuildingTooltipUserWidget::UpdateTooltipBuilding(UBuildingData* Building, bool bIsBuilt)
{
	TitleText->SetText(FText::FromString(Building->BuildingName));

	auto Tier = UEnum::GetDisplayValueAsText(Building->BuildingTier).ToString();
	if(Tier != "Expand" && Tier != "Build" && Tier != "Deconstruct")
	{
		FString Prefix = "Tier ";
		FString CombinedString;
		CombinedString.Append(Prefix += Tier);
		TierText->SetText(FText::FromString(CombinedString));
		TierText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TierText->SetVisibility(ESlateVisibility::Collapsed);
	}
	
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

	/* Resource Cost to build - only show these if the building isn't yet built */
	if(!bIsBuilt)
	{
		for(FResourceData& Resource : Building->ResourcesToBuild)
		{
			FString ResourceEnumString = UEnum::GetDisplayValueAsText(Resource.Resource).ToString();
			FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
			FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
			FString ResourceDetailString = "<Negative> -" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";

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
	}
	
	/* Resources Produced or Consumed per turn */
	for(FResourceData& Resource : Building->ResourcesPerTurn)
	{
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(Resource.Resource).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailStringPos = "<Positive> +" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";
		FString ResourceDetailStringNeg = "<Negative>" + FString::SanitizeFloat(Resource.ResourceAmount) + "</>";
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

void UBuildingTooltipUserWidget::UpdateTooltipArmy(AArmyUnit* Army, bool bIsSpawned)
{
	TitleText->SetText(FText::FromString(Army->GetArmyName()));
	
	auto Tier = UEnum::GetDisplayValueAsText(Army->GetDefaultUnitData()->GetUnitTier()).ToString();
	FString Prefix = "Tier ";
	FString CombinedString;
	CombinedString.Append(Prefix += Tier);
	TierText->SetText(FText::FromString(CombinedString));
	TierText->SetVisibility(ESlateVisibility::Visible);
	
	bool DescriptionIsEmpty = true;
	bool ResourceCostIsEmpty = true;
	bool PerTurnIsEmpty = true;
	
	FString Description;
	FString ResourceCostDescription;
	FString ResourcePerTurnDescription;
	
	Description += Army->GetArmyDescription();
	if(Army->GetArmyDescription().Len() < 1)
	{
		DescriptionIsEmpty = true;
	}
	else
	{
		DescriptionIsEmpty = false;
	}

	/* Resource Cost to build - only show these if the building isn't yet built */
	if(!bIsSpawned)
	{
		/* Gold cost */
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailString = "<Negative> -" + FString::SanitizeFloat(Army->GetGoldCost()) + "</>";

		/* We don't need a new line if it's the first line of the description */
		if(ResourceCostDescription.IsEmpty())
		{
			ResourceCostDescription += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}
		else /* We do want a new line otherwise */
		{
			ResourceCostDescription += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}

		/* Food cost */
		ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Food).ToString();
		ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		ResourceDetailString = "<Negative> -" + FString::SanitizeFloat(Army->GetFoodCost()) + "</>";

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
	
	/* Resources Consumed per turn */
	/* Gold */
	FString ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
	FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
	FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
	FString ResourceDetailStringPos = "<Positive> +" + FString::SanitizeFloat(Army->GetGoldUpkeep()) + "</>";
	FString ResourceDetailStringNeg = "<Negative>" + FString::SanitizeFloat(Army->GetGoldUpkeep()) + "</>";
	FString ResourceDetailStringToUse = Army->GetGoldUpkeep() > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;

	/* We don't need a new line if it's the first line of the description */
	if(ResourcePerTurnDescription.IsEmpty())
	{
		ResourcePerTurnDescription += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
	}
	else /* We do want a new line otherwise */
	{
		ResourcePerTurnDescription += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
	}

	/* Food */
	ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Food).ToString();
	ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
	ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
	ResourceDetailStringPos = "<Positive> +" + FString::SanitizeFloat(Army->GetFoodUpkeep()) + "</>";
	ResourceDetailStringNeg = "<Negative>" + FString::SanitizeFloat(Army->GetFoodUpkeep()) + "</>";
	ResourceDetailStringToUse = Army->GetFoodUpkeep() > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;

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

void UBuildingTooltipUserWidget::UpdateTooltipUnit(UUnitData* Unit, bool bIsSpawned, bool bIsGarrisoned)
{
	TitleText->SetText(FText::FromName(Unit->GetUnitName()));
	
	auto Tier = UEnum::GetDisplayValueAsText(Unit->GetUnitTier()).ToString();
	FString Prefix = "Tier ";
	FString CombinedString;
	CombinedString.Append(Prefix += Tier);
	TierText->SetText(FText::FromString(CombinedString));
	TierText->SetVisibility(ESlateVisibility::Visible);
	
	bool DescriptionIsEmpty = true;
	bool ResourceCostIsEmpty = true;
	bool PerTurnIsEmpty = true;
	
	FString Description;
	FString ResourceCostDescription;
	FString ResourcePerTurnDescription;
	
	Description += Unit->GetUnitDescription();
	if(Unit->GetUnitDescription().Len() < 1)
	{
		DescriptionIsEmpty = true;
	}
	else
	{
		DescriptionIsEmpty = false;
	}

	/* Resource Cost to build - only show these if the building isn't yet built */
	if(!bIsSpawned)
	{
		/* Gold cost */
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailString = "<Negative> -" + FString::SanitizeFloat(Unit->GetGoldCostToCreate()) + "</>";

		/* We don't need a new line if it's the first line of the description */
		if(ResourceCostDescription.IsEmpty())
		{
			ResourceCostDescription += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}
		else /* We do want a new line otherwise */
		{
			ResourceCostDescription += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailString;
		}

		/* Food cost */
		ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Food).ToString();
		ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		ResourceDetailString = "<Negative> -" + FString::SanitizeFloat(Unit->GetFoodCostToCreate()) + "</>";

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
	
	/* Resources Consumed per turn */
	if(!bIsGarrisoned)
	{
		/* Gold */
		FString ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
		FString ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		FString ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		FString ResourceDetailStringPos = "<Positive> +" + FString::SanitizeFloat(Unit->GetGoldUpkeep()) + "</>";
		FString ResourceDetailStringNeg = "<Negative>" + FString::SanitizeFloat(Unit->GetGoldUpkeep()) + "</>";
		FString ResourceDetailStringToUse = Unit->GetGoldUpkeep() > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;

		/* We don't need a new line if it's the first line of the description */
		if(ResourcePerTurnDescription.IsEmpty())
		{
			ResourcePerTurnDescription += ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
		}
		else /* We do want a new line otherwise */
		{
			ResourcePerTurnDescription += "\n" + ResourceIconString + " " + ResourceNameString + " " + ResourceDetailStringToUse;
		}

		/* Food */
		ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Food).ToString();
		ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		ResourceDetailStringPos = "<Positive> +" + FString::SanitizeFloat(Unit->GetFoodUpkeep()) + "</>";
		ResourceDetailStringNeg = "<Negative>" + FString::SanitizeFloat(Unit->GetFoodUpkeep()) + "</>";
		ResourceDetailStringToUse = Unit->GetFoodUpkeep() > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;

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

void UBuildingTooltipUserWidget::UpdateTooltipForced(FString Title, FString Description)
{
	TitleText->SetText(FText::FromString(Title));
	BuildingDescriptionText->SetText(FText::FromString(Description));
	Description.IsEmpty() ? DescriptionLayout->SetVisibility(ESlateVisibility::Collapsed) : DescriptionLayout->SetVisibility(ESlateVisibility::Visible);
	CostLayout->SetVisibility(ESlateVisibility::Collapsed);
	OngoingLayout->SetVisibility(ESlateVisibility::Collapsed);
}
