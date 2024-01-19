// Fill out your copyright notice in the Description page of Project Settings.

#pragma warning(disable:4800)

#include "Settlements/Settlement.h"

#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Settlements/BuildingData.h"

// Sets default values
ASettlement::ASettlement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
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
					UE_LOG(LogTemp, Warning, TEXT("Build Settlement"));
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
			UE_LOG(LogTemp, Warning, TEXT("Build Empty"));
		}
		else
		{
			BuildBuilding(ExpandBuilding, i);
			UE_LOG(LogTemp, Warning, TEXT("Build Expand"));
		}
	}
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
}

void ASettlement::OnNextTurn()
{
	/* TODO: Add ability to collect resources from buildings */
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

TArray<UBuildingData*> ASettlement::GetBuildingsToBuild()
{
	TArray<UBuildingData*> Temp;
	/* Add tier 1 farm buildings */
	Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Farming, EBuildingTier::Tier1));

	/* Add tier 1 garrison buildings */
	Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Military, EBuildingTier::Tier1));

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

void ASettlement::UpdateBuildingCapAvailable()
{
	UBuildingData* Building = CurrentBuildings[0];
	if(Building && Building->BuildingType == EBuildingType::Settlement)
	{
		if(Building->BuildingModifiers.Num() > 0)
		{
			for(FLocalResourceData Data : Building->BuildingModifiers)
			{
				if(Data.Bonus == ELocalResourceType::BuildingCap)
				{
					UE_LOG(LogTemp, Warning, TEXT("Build Cap Updated! %f"), Data.BonusValue);
					BuildingCapAvailable = Data.BonusValue;
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
		if(!CanAfford) return false;
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


void ASettlement::BuildBuilding(UBuildingData* Building, int Index)
{
	if(Building)
	{
		if(CheckCanAffordBuilding(Building))
		{
			CurrentBuildings[Index] = Building;
		}
		
		/* Update the building cap if this is a settlement building */
		if(Building->BuildingType == EBuildingType::Settlement)
		{
			UpdateBuildingCapAvailable();
		}
	}
}

