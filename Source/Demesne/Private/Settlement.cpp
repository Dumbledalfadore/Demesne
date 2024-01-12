// Fill out your copyright notice in the Description page of Project Settings.

#pragma warning(disable:4800)

#include "Settlement.h"
#include "BuildingData.h"

// Sets default values
ASettlement::ASettlement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	//Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	//Collider->SetBoxExtent(FVector(100.0, 100.0, 100.0));
	//Collider->SetupAttachment(RootComponent);
	//Collider->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASettlement::BeginPlay()
{
	Super::BeginPlay();

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

	/* By default all settlements should have a settlement building in the first slot */
	if (SettlementBuildings.Num() > 0)
	{
		/* Get the lowest tier building in the settlement building list and build it */
		for (auto Building : SettlementBuildings)
		{
			if(Building->BuildingTier == EBuildingTier::Tier0)
			{
				if(BuildingCount < BuildingCap)
				{
					BuildBuilding(Building, 0);
				}
			}
		}
	}
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

// Called every frame
void ASettlement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	switch (Building->BuildingTier)
	{
	case EBuildingTier::Tier0:
		return EBuildingTier::Tier1;
	case EBuildingTier::Tier1:
		return EBuildingTier::Tier2;
	case EBuildingTier::Tier2:
		return EBuildingTier::Tier3;
	case EBuildingTier::Tier3:
		return EBuildingTier::Tier4;
	case EBuildingTier::Tier4:
		return EBuildingTier::Tier4;
	default:
		break;
	}

	return EBuildingTier::Tier0;
}

TArray<UBuildingData*> ASettlement::GetBuildingsToBuild()
{
	TArray<UBuildingData*> Temp;
	/* Add tier 1 farm buildings */
	Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Farming, EBuildingTier::Tier0));

	/* Add tier 1 garrison buildings */
	Temp.Append(GetBuildingsByTypeAndTier(EBuildingType::Military, EBuildingTier::Tier0));

	TArray<UBuildingData*> NewTemp;

	for(int i = 0; i < Temp.Num(); i++)
	{
		/* If it's not built, add it */
		if(!CheckAlreadyBuilt(Temp[i]))
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
				/* Check if the identifier from the upgrades already exists in the current buildings
				 * and also check if this building has the same identifier - both are valid upgrades  */
				if(!CheckMatchingIdentifier(Data, Temp[i]) || CheckMatchingIdentifier(BuildingData, Temp[i]))
				{
					/* If it doesn't exist then it's a valid upgrade*/
					CanBeBuilt = true;
				}
			}

			if(CanBeBuilt)
			{
				NewTemp.Add(Temp[i]);
			}
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

bool ASettlement::CheckAlreadyBuilt(UBuildingData* BuildingData)
{
	for(int i = 0; i < CurrentBuildings.Num(); i++)
	{		
		if(CurrentBuildings[i] && BuildingData == CurrentBuildings[i])
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
	for(int i = 0; i < CurrentBuildings.Num(); i++)
	{
		if(CurrentBuilding && UpgradeBuilding && CurrentBuilding->BuildingIdentifier == UpgradeBuilding->BuildingIdentifier)
		{
			/* Found a match */
			return true;
		}
	}

	/* No Match */
	return false;
}


void ASettlement::BuildBuilding(UBuildingData* Building, int Index)
{
	if(Building)
	{
		/* TODO: Add checks to make sure we have the resources to build it and then remove them */
		CurrentBuildings[Index] = Building;
	}
}

