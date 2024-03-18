// Created by WarRoom Wizards ©2023


#include "Military/ArmyDataComponent.h"

#include "Military/UnitData.h"

// Sets default values for this component's properties
UArmyDataComponent::UArmyDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UArmyDataComponent::CheckCanAddUnit()
{
	return CurrentUnitData.Num() < MaxUnitsInArmy;
}

void UArmyDataComponent::AddUnit(UUnitData* UnitData)
{
	if(!CheckCanAddUnit()) return; /* No space -- Should already have been checked but double check here */

	//UE_LOG(LogTemp, Warning, TEXT("UNIT ADDED: %s"), *UnitData->GetUnitName().ToString())
	CurrentUnitData.Add(UnitData);
	CurrentUnitHealth.Add(UnitData->GetMaxHealth());

	CalculateUpkeeps();
}

void UArmyDataComponent::AddUnits(TArray<UUnitData*> Units)
{
	for(auto Unit : Units)
	{
		if(Unit)
		{
			AddUnit(Unit);
		}
	}
}


float UArmyDataComponent::GetUnitHealth(const int Index)
{
	return CurrentUnitHealth[Index];
}

bool UArmyDataComponent::CheckUnitIsAlive(const int Index)
{
	if(GetUnitHealth(Index) <= 0)
	{
		return false;
	}

	return true;
}

void UArmyDataComponent::RemoveUnitAtIndex(const int Index)
{
	if(CurrentUnitData.Num() - 1 < Index || CurrentUnitHealth.Num() - 1 < Index) return;
	
	CurrentUnitData.RemoveAt(Index);
	CurrentUnitHealth.RemoveAt(Index);

	CalculateUpkeeps();
}

void UArmyDataComponent::RemoveDeadUnits()
{
	for(int i = 0; i < CurrentUnitData.Num() - 1; i++)
	{
		if(CurrentUnitHealth[i])
		{
			if(CheckUnitIsAlive(i))
			{
				RemoveUnitAtIndex(i);
				/* Unit was removed and array was shrunk so we need to go back to the previous index */
				i--;
			}
		}
	}
}

void UArmyDataComponent::RemoveUnit(UUnitData* UnitData)
{
	int Index = CurrentUnitData.Find(UnitData);
	RemoveUnitAtIndex(Index);
}

void UArmyDataComponent::RemoveUnits(TArray<UUnitData*> Units)
{
	for(auto Unit : Units)
	{
		if(Unit)
		{
			RemoveUnit(Unit);
		}
	}
}

void UArmyDataComponent::ClearUnits()
{
	CurrentUnitData.Empty();
	CurrentUnitHealth.Empty();

	CalculateUpkeeps();
}

float UArmyDataComponent::CalculateTotalFoodUpkeep()
{
	float Upkeep = 0;
	for(auto Unit : CurrentUnitData)
	{
		Upkeep += Unit->GetFoodUpkeep();
	}

	//UE_LOG(LogTemp, Warning, TEXT("FOOD UPKEEP: %f"), Upkeep);
	return Upkeep;
}

float UArmyDataComponent::CalculateTotalGoldUpkeep()
{
	float Upkeep = 0;
	for(auto Unit : CurrentUnitData)
	{
		Upkeep += Unit->GetGoldUpkeep();
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("GOLD UPKEEP: %f"), Upkeep);
	return Upkeep;
}


void UArmyDataComponent::CalculateUpkeeps()
{
	FoodUpkeep = CalculateTotalFoodUpkeep();
	GoldUpkeep = CalculateTotalGoldUpkeep();
}

// Called when the game starts
void UArmyDataComponent::BeginPlay()
{
	Super::BeginPlay();

	CalculateUpkeeps(); /* Reset */
	
}

