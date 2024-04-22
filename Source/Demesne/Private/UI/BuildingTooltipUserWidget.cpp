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
	bool UnitsIsEmpty = true;
	
	FString Description;
	FString ResourceCostDescription;
	FString ResourcePerTurnDescription;
	FString UnitsDescription;
	
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

	if(Building->GarrisonUnits.Num() != 0)
	{
		/* Stores the amount of each unit type */
		TMap<FString, int> UnitCounts;

		for(auto Unit : Building->GarrisonUnits)
		{
			FString Key = Unit->GetUnitName().ToString();
			if(UnitCounts.Contains(Key))
			{
				int CurValue = *UnitCounts.Find(Key);
				
				/* Increase the amount */
				CurValue++;
				
				UnitCounts.Emplace(Key, CurValue);
			}
			else
			{
				UnitCounts.Add(Key, 1);
			}
		}

		for(auto Units : UnitCounts)
		{
			if(UnitsDescription.IsEmpty())
			{
				UnitsDescription += Units.Key + " <Multiply>x</>" + FString::FromInt(Units.Value);
			}
			else /* We do want a new line otherwise */
			{
				UnitsDescription += "\n" + Units.Key + " <Multiply>x</>" + FString::FromInt(Units.Value);
			}
		}
		
		UnitsIsEmpty = false;
	}
	
	/* Removes the empty line from the end */
	Description.TrimEndInline();
	ResourceCostDescription.TrimEndInline();
	ResourcePerTurnDescription.TrimEndInline();

	DescriptionIsEmpty == true ? DescriptionLayout->SetVisibility(ESlateVisibility::Collapsed) : DescriptionLayout->SetVisibility(ESlateVisibility::Visible);
	ResourceCostIsEmpty == true ? CostLayout->SetVisibility(ESlateVisibility::Collapsed) : CostLayout->SetVisibility(ESlateVisibility::Visible);
	PerTurnIsEmpty == true ? OngoingLayout->SetVisibility(ESlateVisibility::Collapsed) : OngoingLayout->SetVisibility(ESlateVisibility::Visible);
	UnitsIsEmpty == true ? UnitsLayout->SetVisibility(ESlateVisibility::Collapsed) : UnitsLayout->SetVisibility(ESlateVisibility::Visible);

	BuildingDescriptionText->SetText(FText::FromString(Description));
	CostDescriptionText->SetText(FText::FromString(ResourceCostDescription));
	TurnCostDescriptionText->SetText(FText::FromString(ResourcePerTurnDescription));
	UnitsText->SetText(FText::FromString(UnitsDescription));
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
	bool UnitsIsEmpty = true;
	
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

	/* Resource Cost to spawn - only show these if the army isn't yet spawned */
	if(!bIsSpawned)
	{
		FString ResourceEnumString;
		FString ResourceIconString;
		FString ResourceNameString;
		FString ResourceDetailString;
		/* Gold cost */
		if(Army->GetGoldCost() > 0)
		{
			ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
			ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
			ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
			ResourceDetailString = "<Negative> -" + FString::SanitizeFloat(Army->GetGoldCost()) + "</>";

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

		/* Food cost */
		if(Army->GetFoodCost() > 0)
		{
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
	}
		
	
	FString ResourceEnumString;
	FString ResourceIconString;
	FString ResourceNameString;
	FString ResourceDetailStringPos;
	FString ResourceDetailStringNeg;
	FString ResourceDetailStringToUse;
	/* Resources Consumed per turn */
	/* Gold */

	if(Army->GetGoldUpkeep() > 0)
	{
		ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
		ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
		ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
		ResourceDetailStringPos = "<Positive> +" + FString::SanitizeFloat(Army->GetGoldUpkeep()) + "</>";
		ResourceDetailStringNeg = "<Negative>" + FString::SanitizeFloat(Army->GetGoldUpkeep()) + "</>";
		ResourceDetailStringToUse = Army->GetGoldUpkeep() > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;
		
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

	/* Food */
	if(Army->GetFoodUpkeep() > 0)
	{
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
	}
	
	/* Removes the empty line from the end */
	Description.TrimEndInline();
	ResourceCostDescription.TrimEndInline();
	ResourcePerTurnDescription.TrimEndInline();

	DescriptionIsEmpty == true ? DescriptionLayout->SetVisibility(ESlateVisibility::Collapsed) : DescriptionLayout->SetVisibility(ESlateVisibility::Visible);
	ResourceCostIsEmpty == true ? CostLayout->SetVisibility(ESlateVisibility::Collapsed) : CostLayout->SetVisibility(ESlateVisibility::Visible);
	PerTurnIsEmpty == true ? OngoingLayout->SetVisibility(ESlateVisibility::Collapsed) : OngoingLayout->SetVisibility(ESlateVisibility::Visible);
	UnitsIsEmpty == true ? UnitsLayout->SetVisibility(ESlateVisibility::Collapsed) : UnitsLayout->SetVisibility(ESlateVisibility::Visible);

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
	bool UnitsIsEmpty = true;
	
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

	/* Resource Cost to spawn - only show these if the unit isn't yet spawned */
	if(!bIsSpawned)
	{
		/* Gold cost */
		FString ResourceEnumString;
		FString ResourceIconString;
		FString ResourceNameString;
		FString ResourceDetailString;

		if(Unit->GetGoldCostToCreate() > 0)
		{
			ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
			ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
			ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
			ResourceDetailString = "<Negative> -" + FString::SanitizeFloat(Unit->GetGoldCostToCreate()) + "</>";
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

		/* Food cost */
		if(Unit->GetFoodCostToCreate() > 0)
		{
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
	}
	
	/* Resources Consumed per turn */
	if(!bIsGarrisoned)
	{
		/* Gold */
		FString ResourceEnumString;
		FString ResourceIconString;
		FString ResourceNameString;
		FString ResourceDetailStringPos;
		FString ResourceDetailStringNeg;
		FString ResourceDetailStringToUse;

		if(Unit->GetGoldUpkeep() > 0)
		{
			ResourceEnumString = UEnum::GetDisplayValueAsText(EResourceType::Gold).ToString();
			ResourceIconString = "<img id=\"" + ResourceEnumString + "\"/>";
			ResourceNameString = "<Resource>" + ResourceEnumString + "</>";
			ResourceDetailStringPos = "<Positive> +" + FString::SanitizeFloat(Unit->GetGoldUpkeep()) + "</>";
			ResourceDetailStringNeg = "<Negative>" + FString::SanitizeFloat(Unit->GetGoldUpkeep()) + "</>";
			ResourceDetailStringToUse = Unit->GetGoldUpkeep() > 0 ? ResourceDetailStringPos : ResourceDetailStringNeg;
			
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

		/* Food */
		if(Unit->GetFoodUpkeep() > 0)
		{
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
	}
	
	/* Removes the empty line from the end */
	Description.TrimEndInline();
	ResourceCostDescription.TrimEndInline();
	ResourcePerTurnDescription.TrimEndInline();

	DescriptionIsEmpty == true ? DescriptionLayout->SetVisibility(ESlateVisibility::Collapsed) : DescriptionLayout->SetVisibility(ESlateVisibility::Visible);
	ResourceCostIsEmpty == true ? CostLayout->SetVisibility(ESlateVisibility::Collapsed) : CostLayout->SetVisibility(ESlateVisibility::Visible);
	PerTurnIsEmpty == true ? OngoingLayout->SetVisibility(ESlateVisibility::Collapsed) : OngoingLayout->SetVisibility(ESlateVisibility::Visible);
	UnitsIsEmpty == true ? UnitsLayout->SetVisibility(ESlateVisibility::Collapsed) : UnitsLayout->SetVisibility(ESlateVisibility::Visible);

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
