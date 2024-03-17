// Fill out your copyright notice in the Description page of Project Settings.

#pragma warning(disable:4800)

#include "Settlements/Settlement.h"

#include "EconHelper.h"
#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyDataComponent.h"
#include "Settlements/BuildingData.h"

// Sets default values
ASettlement::ASettlement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	GarrisonComponent = CreateDefaultSubobject<UArmyDataComponent>(TEXT("Garrison Component"));
}

void ASettlement::ResetSettlement()
{
	/* By default all settlements should have a settlement building in the first slot */
	if (SettlementBuildings.Num() > 0)
	{
		/* Get the lowest tier building in the settlement building list and build it */
		for (auto Building : SettlementBuildings)
		{
			if(Building->BuildingTier == EBuildingTier::Tier1)
			{
				if(BuildingCount < BuildingCap)
				{
					BuildBuilding(Building, 0);
				}
			}
		}
	}

	/* Set each slot to an expansion slot
	 * Index from 1 so we dont write over the first slot */
	for (int i = 1; i < CurrentBuildings.Num(); i++)
	{
		if(i < GetBuildingCapAvailable())
		{
			BuildBuilding(EmptyBuilding, i);
		}
		else
		{
			BuildBuilding(ExpandBuilding, i);
		}
	}

	/* Should always start at one */
	SettlementPopulation = 1;
	
	GrowthForNextPop = EconHelper::CalculateGrowthForNextLevel(SettlementPopulation);

	/* Empty the garrison */
	GarrisonComponent->ClearUnits();

	RecalculateValues();
}

// Called when the game starts or when spawned
void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	
	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	/* Cleanup the building arrays of duplicates */
	SettlementBuildings = RemoveDuplicateBuildings(SettlementBuildings);
	FarmBuildings = RemoveDuplicateBuildings(FarmBuildings);
	MilitaryBuildings = RemoveDuplicateBuildings(MilitaryBuildings);
	FishingBuildings = RemoveDuplicateBuildings(FishingBuildings);
	CraftBuildings = RemoveDuplicateBuildings(CraftBuildings);
	ReligiousBuildings = RemoveDuplicateBuildings(ReligiousBuildings);

	/* Switches the amount of building slots the settlement type has */
	switch (SettlementType)
	{
	case ESettlementType::Camp:
		BuildingCap = 2;
		break;
	case ESettlementType::Minor:
		BuildingCap = 4;
		break;
	case ESettlementType::Major:
		BuildingCap = 8;
		break;
	default:
		break;
	}

	/* Sets the settlement to default state.
	 * TODO: Modify this when/if saving functionality gets added.
	 */
	ResetSettlement();

	/* Bind turn manager delegate */
	if(GM && GM->EconComp && GM->EconComp->TurnManagerRef)
	{
		GM->EconComp->TurnManagerRef->OnTurnEnd.AddUniqueDynamic(this, &ThisClass::OnNextTurn);
	}
}

void ASettlement::OnNextTurn()
{
	/* Add the growth to accumulated growth */
	AccumulatedGrowth += CurrentGrowthRate;

	/* Check if we have passed the new pop threshold */
	if(AccumulatedGrowth >= GrowthForNextPop)
	{
		/* Remove old food upkeep value */
		GM->EconComp->SubtractFoodUpkeep(PlayerID, CurrentFoodUpkeep);
		
		/* Increase the population*/
		SettlementPopulation++;

		/* Set new food upkeep value and apply it */
		/* To keep it simple 1 pop = 1 food upkeep */
		CurrentFoodUpkeep += 1;
		GM->EconComp->AddFoodUpkeep(PlayerID, CurrentFoodUpkeep);
		

		/* Remove the amount of growth that was needed */
		AccumulatedGrowth -= GrowthForNextPop;
		
		GrowthForNextPop = EconHelper::CalculateGrowthForNextLevel(SettlementPopulation);

	}
}

void ASettlement::RecalculateValues()
{
	if(!GM || !GM->EconComp) return;
	
	/* Remove old values, could be 0 */
	GM->EconComp->SubtractGoldIncome(PlayerID, CurrentGoldIncome);
	GM->EconComp->SubtractFoodIncome(PlayerID, CurrentFoodIncome);
	GM->EconComp->SubtractGoldUpkeep(PlayerID, CurrentGoldUpkeep);
	GM->EconComp->SubtractFoodUpkeep(PlayerID, CurrentFoodUpkeep);

	/* Update the values */
	CurrentGoldIncome = GetLocalGoldIncome();
	CurrentFoodIncome = GetLocalFoodIncome();
	CurrentGrowthRate = GetLocalGrowth();
	CurrentFoodUpkeep = GetLocalFoodUpkeep();
	CurrentGoldUpkeep = GetLocalGoldUpkeep();

	/* Add the new values */
	GM->EconComp->AddGoldIncome(PlayerID, CurrentGoldIncome);
	GM->EconComp->AddFoodIncome(PlayerID, CurrentFoodIncome);
	GM->EconComp->AddGoldUpkeep(PlayerID, CurrentGoldUpkeep);
	GM->EconComp->AddFoodUpkeep(PlayerID, CurrentFoodUpkeep);
	
}

TArray<UBuildingData*> ASettlement::RemoveDuplicateBuildings(TArray<UBuildingData*> Array)
{
	TArray<UBuildingData*> NewArray;
	for(int i = 0; i < Array.Num(); i++)
	{
		auto Temp = Array[i];

		/* Make sure we only add one of each to the array */
		if(!NewArray.Contains(Temp))
		{
			NewArray.Add(Temp);
		}
	}

	return NewArray;
}

UBuildingData* ASettlement::GetBuildingAtIndex(int Index)
{
	if(CurrentBuildings[Index])
	{
		return CurrentBuildings[Index];
	}

	return nullptr;
}

TArray<UBuildingData*> ASettlement::GetBuildingsByType(EBuildingType Type)
{
	switch (Type)
	{
	case EBuildingType::Settlement:
		return SettlementBuildings;
	case EBuildingType::Farming:
		return FarmBuildings;
	case EBuildingType::Military:
		return MilitaryBuildings;
	case EBuildingType::Fishing:
		return FishingBuildings;
	case EBuildingType::Craft:
		return CraftBuildings;
	case EBuildingType::Religious:
		return ReligiousBuildings;
	case EBuildingType::Misc:
		UE_LOG(LogTemp, Error, TEXT("Misc buildings are not accessed via array, access them directly instead with 'ExpandBuilding', 'EmptyBuilding' and 'DeconstructBuilding'!"))
		break;
	default:
		break;
	}
	
	TArray<UBuildingData*> Temp;
	return Temp;
}

TArray<UBuildingData*> ASettlement::GetBuildingsByTypeAndTier(EBuildingType Type, EBuildingTier Tier)
{
	TArray<UBuildingData*> Temp = GetBuildingsByType(Type);

	/* We will add any we want to keep to this array */
	TArray<UBuildingData*> NewTemp;

	for(int i = 0; i < Temp.Num(); i++)
	{
		/* If it's the correct tier and we don't already have it, add to the new array */
		if(Tier == Temp[i]->BuildingTier && !CheckAlreadyBuilt(Temp[i]))
		{
			NewTemp.Add(Temp[i]);
		}
	}

	return NewTemp;
}

EBuildingTier ASettlement::GetNextBuildingTier(UBuildingData* Building)
{
	if(!Building) return EBuildingTier::Expand;
	
	switch (Building->BuildingTier)
	{
	case EBuildingTier::Expand:
		return EBuildingTier::Build;
	case EBuildingTier::Build:
		return EBuildingTier::Tier1;
	case EBuildingTier::Tier1:
		return EBuildingTier::Tier2;
	case EBuildingTier::Tier2:
		return EBuildingTier::Tier3;
	case EBuildingTier::Tier3:
		return EBuildingTier::Tier4;
	case EBuildingTier::Tier4:
		return EBuildingTier::Tier5;
	case EBuildingTier::Tier5:
		return EBuildingTier::Tier5;
	default:
		return EBuildingTier::Expand;
	}
}

EBuildingTier ASettlement::GetBuildingTier(UBuildingData* Building)
{
	if(!Building) return EBuildingTier::Expand;

	return Building->BuildingTier;
}

EBuildingTier ASettlement::GetBuildingTierCap()
{
	switch (SettlementType) {
	case ESettlementType::Camp:
		return EBuildingTier::Tier2;
	case ESettlementType::Minor:
		return EBuildingTier::Tier4;
	case ESettlementType::Major:
		return EBuildingTier::Tier5;
	}
	return EBuildingTier::Tier1;
}

TArray<UBuildingData*> ASettlement::GetBuildingsToBuild()
{
	TArray<UBuildingData*> Temp;
	/* Add tier 1 farm buildings */
	Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Farming, EBuildingTier::Tier1));
	/* Add tier 1 garrison buildings */
	Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Military, EBuildingTier::Tier1));
	/* Add tier 1 fishing buildings but only if this is a coastal settlement */
	if(bIsCoastal)
	{
		Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Fishing, EBuildingTier::Tier1));
	}
	/* Add tier 1 craft buildings */
	Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Craft, EBuildingTier::Tier1));

	TArray<UBuildingData*> NewTemp;

	bool CanBeBuilt = false;
	for(int i = 0; i < Temp.Num(); i++)
	{
		CanBeBuilt = false;
		for(UBuildingData* Data : CurrentBuildings)
		{
			/* Check if the current building exists, else continue the loop */
			if(!Data)
			{
				continue;
			}
						
			/* If it's not built, it can be */
			if(!CheckAlreadyBuilt(Temp[i]) && !CheckMatchingIdentifier(Data, Temp[i]))
			{
				CanBeBuilt = true;
			}
			else
			{
				CanBeBuilt = false;
				break; /* We don't need to continue in this loop */
			}
		}

		if(CanBeBuilt)
		{
			NewTemp.Add(Temp[i]);
		}
	}

	return NewTemp;
}

TArray<UBuildingData*> ASettlement::GetUpgradeBuildings(UBuildingData* BuildingData)
{
	TArray<UBuildingData*> Temp; /* Array to hold the upgrades */
	TArray<UBuildingData*> NewTemp; /* Array to hold the valid upgrades after we've check them */
	
	if(BuildingData)
	{
		/* We don't want to upgrade a settlement building further than the imposed cap for the settlement type */
		if(BuildingData->BuildingType == EBuildingType::Settlement)
		{
			if(BuildingData->BuildingTier >= GetBuildingTierCap()) return Temp;
		}
		else
		{
			/* We can only continue if the current building tier is lower than the tier of the settlement building
			 * We wouldn't want a camp that's tier 2 to be able to build for example a tier 4/5 farm */
			if(BuildingData->BuildingTier >= GetBuildingTier(CurrentBuildings[0]))
			{
				/* If there is an actual building, we should be able to deconstruct it, unless its a settlement building */
				if(DeconstructBuilding && BuildingData->BuildingType != EBuildingType::Misc && BuildingData->BuildingType != EBuildingType::Settlement)
				{
					NewTemp.Add(DeconstructBuilding);
				}
				return NewTemp;
			}
		}
		
		Temp = BuildingData->BuildingUpgrades;
		
		/* Track if this upgrade can be built */
		bool CanBeBuilt = false;
		for(int i = 0; i < Temp.Num(); i++)
		{
			CanBeBuilt = false;
			for(UBuildingData* Data : CurrentBuildings)
			{
				/* Check if the current building exists, else continue the loop */
				if(!Data)
				{
					continue;
				}
				
				/* Check if the identifier from the upgrades already exists in the current buildings, if so we can't build it */
				if(!CheckMatchingIdentifier(Data, Temp[i]))
				{
					CanBeBuilt = true;
				} /* Check if the identifier matches the selected building, if so then we can build it */
				else if (CheckMatchingIdentifier(BuildingData, Temp[i]))
				{
					CanBeBuilt = true;
				}
				else
				{
					CanBeBuilt = false;
					break; /* We don't need to continue in this loop */
				}
			}

			if(CanBeBuilt)
			{
				NewTemp.Add(Temp[i]);
			}
		}

		/* If there is an actual building, we should be able to deconstruct it, unless its a settlement building */
		if(DeconstructBuilding && BuildingData->BuildingType != EBuildingType::Misc && BuildingData->BuildingType != EBuildingType::Settlement)
		{
			NewTemp.Add(DeconstructBuilding);
		}
	}

	return NewTemp;
}

TArray<UBuildingData*> ASettlement::GetCurrentBuildings()
{
	TArray<UBuildingData*> Temp;
	Temp.Append(CurrentBuildings);
	return Temp;
}

float ASettlement::GetResourceValue(UBuildingData* Building, EResourceType Resource)
{
	if(!Building) return 0.0f;

	for(auto Res : Building->ResourcesPerTurn)
	{
		if(Res.Resource == Resource)
		{
			return Res.ResourceAmount;
		}
	}

	return 0.0f;
}

float ASettlement::GetLocalResourceValue(UBuildingData* Building, ELocalResourceType Resource)
{
	if(!Building) return 0.0f;

	for(auto LocalRes : Building->BuildingModifiers)
	{
		if(LocalRes.Resource == Resource)
		{
			return LocalRes.ResourceAmount;
		}
	}

	return 0.0f;
}

float ASettlement::GetLocalFoodIncome()
{
	float Value = 0.0f;
	for(UBuildingData* Building : CurrentBuildings)
	{
		int Temp = GetResourceValue(Building, EResourceType::Food);
		if(Temp > 0)
		{
			Value += GetResourceValue(Building, EResourceType::Food);
		}
	}

	return Value;
}

float ASettlement::GetLocalGoldIncome()
{
	float Value = 0.0f;
	for(UBuildingData* Building : CurrentBuildings)
	{
		int Temp = GetResourceValue(Building, EResourceType::Gold);
		if(Temp > 0)
		{
			Value += GetResourceValue(Building, EResourceType::Gold);
		}
	}

	return Value;
}

float ASettlement::GetLocalGrowth()
{
	float Value = EconHelper::BaseGrowthRate;
	for(UBuildingData* Building : CurrentBuildings)
	{
		Value += GetLocalResourceValue(Building, ELocalResourceType::Growth);
	}

	return Value;
}

float ASettlement::GetLocalGoldUpkeep()
{
	float Value = 0.0f;

	/* Add the current population as upkeep 1:1 */
	Value += GetPopulation();

	for(UBuildingData* Building : CurrentBuildings)
	{
		int Temp = GetResourceValue(Building, EResourceType::Gold);
		if(Temp < 0)
		{
			Value += GetResourceValue(Building, EResourceType::Gold);
		}
	}
	
	Value += GarrisonComponent->GetGoldUpkeep();

	return Value;
}

float ASettlement::GetLocalFoodUpkeep()
{
	float Value = 0.0f;
	for(UBuildingData* Building : CurrentBuildings)
	{
		int Temp = GetResourceValue(Building, EResourceType::Food);
		if(Temp > 0)
		{
			Value += GetResourceValue(Building, EResourceType::Food);
		}
	}
	
	Value += GarrisonComponent->GetFoodUpkeep();

	return Value;
}

void ASettlement::SetTileIndex(const FIntPoint Index)
{
	TileIndex = Index;
}

void ASettlement::UpdateBuildingCapAvailable()
{
	UBuildingData* Building = CurrentBuildings[0];
	if(Building && Building->BuildingType == EBuildingType::Settlement)
	{
		if(Building->BuildingModifiers.Num() > 0)
		{
			for(FLocalResourceData Data : Building->BuildingModifiers)
			{
				if(Data.Resource == ELocalResourceType::BuildingCap)
				{
					BuildingCapAvailable = Data.ResourceAmount;
					return; /* No need to check any others, there should only be one */
				}
			}
		}
	}
}

bool ASettlement::CheckAlreadyBuilt(UBuildingData* BuildingData)
{
	for(int i = 0; i < CurrentBuildings.Num(); i++)
	{		
		if(CurrentBuildings[i] && BuildingData == CurrentBuildings[i] && BuildingData->BuildingType != EBuildingType::Misc)
		{
			/* It's already built*/
			return true;
		}
	}

	/* Not built yet */
	return false;
}

bool ASettlement::CheckMatchingIdentifier(UBuildingData* CurrentBuilding, UBuildingData* UpgradeBuilding)
{
	if(CurrentBuilding && UpgradeBuilding && CurrentBuilding->BuildingIdentifier == UpgradeBuilding->BuildingIdentifier && CurrentBuilding->BuildingType != EBuildingType::Misc)
	{
		/* Found a match */
		
		return true;
	}

	/* No Match */
	return false;
}

bool ASettlement::CheckCanAffordBuilding(UBuildingData* Building)
{
	if(!Building) return false;
	if(Building->ResourcesToBuild.Num() == 0) return true; /* Early exit, there is no cost */

	bool CanAfford = true;
	for(const FResourceData& Resource : Building->ResourcesToBuild)
	{
		CanAfford = CheckHasResource(Resource.Resource, Resource.ResourceAmount);
		if(!CanAfford) return false; /* Early exit - if theres even one resource we can't afford, then we can't afford*/
	}

	return true;
}

bool ASettlement::CheckHasResource(EResourceType Resource, float Cost)
{
	if(!GM) return false;
	if(!GM->EconComp) return false;

	switch (Resource)
	{
	case EResourceType::Gold:
		return GM->EconComp->GetGold(PlayerID) >= Cost;
	case EResourceType::Food:
		return GM->EconComp->GetFood(PlayerID) >= Cost;
	default:
		return false;
	}
}

bool ASettlement::CheckHasLocalResource(ELocalResourceType Resource, float Cost)
{
	if(!GM) return false;
	if(!GM->EconComp) return false;

	switch (Resource)
	{
	case ELocalResourceType::Population: /* We just need to check this, we don't need to remove anything */
		return SettlementPopulation >= Cost;
	default:
		return true;
	}
}

void ASettlement::RemoveBuildingResources(UBuildingData* Building)
{
	if(!GM) return;
	if(!GM->EconComp) return;

	for(const FResourceData Resource : Building->ResourcesToBuild)
	{
		switch (Resource.Resource)
		{ /* We dont need to check any resources here as it's done in another function, by this point we should know if we can afford it or not. */
		case EResourceType::Gold:
			GM->EconComp->SetGold(PlayerID, GM->EconComp->GetGold(PlayerID) - Resource.ResourceAmount);
		case EResourceType::Food:
			GM->EconComp->SetFood(PlayerID, GM->EconComp->GetFood(PlayerID) - Resource.ResourceAmount);
		}
	}
}


void ASettlement::BuildBuilding(UBuildingData* Building, int Index)
{
	if(Building)
	{
		if(CheckCanAffordBuilding(Building))
		{
			/* Remove the resources */
			RemoveBuildingResources(Building);

			/* Update the garrison, the old building could have provided units*/
			if(CurrentBuildings[Index] && CurrentBuildings[Index]->GarrisonUnits.Num() > 0)
			{
				GarrisonComponent->RemoveUnits(CurrentBuildings[Index]->GarrisonUnits);
			}

			/* Build the building*/
			CurrentBuildings[Index] = Building;

			/* Update the garrison, the new building might provide units */
			if(CurrentBuildings[Index] && CurrentBuildings[Index]->GarrisonUnits.Num() > 0)
			{
				GarrisonComponent->AddUnits(CurrentBuildings[Index]->GarrisonUnits);
			}

			/* Update the current values so we are removing the correct amount per turn */
			RecalculateValues();

			/* Update the building cap if this is a settlement building */
			if(Building->BuildingType == EBuildingType::Settlement)
			{
				UpdateBuildingCapAvailable();
			}
		}
		else
		{
			OnNotification.Broadcast("You don't have the resources to build this!");
		}
		
	}
}

