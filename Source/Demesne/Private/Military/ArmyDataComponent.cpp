// Created by WarRoom Wizards ©2023


#include "Military/ArmyDataComponent.h"

#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyManager.h"
#include "Military/ArmyUnit.h"
#include "Military/UnitData.h"
#include "Settlements/Settlement.h"
#include "Settlements/SettlementManager.h"

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

	if(GM && GM->EconComp)
	{
		CalculateUpkeeps();
	}
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

	if(GM && GM->EconComp)
	{
		CalculateUpkeeps();
	}
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

	if(GM && GM->EconComp)
	{
		CalculateUpkeeps();
	}
}

float UArmyDataComponent::CalculateTotalFoodUpkeep()
{
	float Upkeep = 0.0f;
	for(auto Unit : CurrentUnitData)
	{
		Upkeep += Unit->GetFoodUpkeep();
	}

	//UE_LOG(LogTemp, Warning, TEXT("FOOD UPKEEP: %f"), Upkeep);
	return Upkeep;
}

float UArmyDataComponent::CalculateTotalGoldUpkeep()
{
	float Upkeep = 0.0f;
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

float UArmyDataComponent::CalculatePower()
{
	float Power = 0.0f;

	int Index = 0;
	for(auto Unit : GetUnitDataArray())
	{
		Power += CalculateUnitPower(Unit, Index);
		Index++;
	}

	return Power;
}

float UArmyDataComponent::CalculateUnitPower(const UUnitData* Unit, const int UnitIndex)
{
	float UnitPower = 0.0f;
	if(!Unit) return UnitPower;
	
	UnitPower += Unit->GetArmour();
	UnitPower += Unit->GetMovementSpeed();
	UnitPower += Unit->GetMaxHealth();
	UnitPower += (Unit->GetMeleeAttackDamage() * Unit->GetMeleeAttackSpeed());
	UnitPower += (Unit->GetRangedAttackDamage() * Unit->GetRangedAttackSpeed());
		

	const float HealthPercentage =  GetUnitHealth(UnitIndex) / Unit->GetMaxHealth();

	/* Unit not at full health so it's not full power */
	UnitPower *= HealthPercentage;

	/* Unit's are different tiers which affects the overall power */
	UnitPower *= (1 + (static_cast<uint8>(Unit->GetUnitTier()) / 5));

	return UnitPower;
}

void UArmyDataComponent::ArmyDecimated(const int ByPlayerID)
{
	//UE_LOG(LogTemp, Warning, TEXT("ARMY DECIMATED BY: %i"), ByPlayerID);
	if(ASettlement* Settlement = Cast<ASettlement>(GetOwner()))
	{
		if(GM && GM->SettlementManager)
		{
			/* Handover the settlement to the victor */
			GM->SettlementManager->HandoverSettlement(Settlement, GetPlayerID(), ByPlayerID);
		}
	}
	else if(AArmyUnit* Army = Cast<AArmyUnit>(GetOwner()))
	{
		if(GM && GM->ArmyManager)
		{
			/* Destroy the army */
			GM->ArmyManager->ArmyDestroyed(Army);
		}
	}
}

// Called when the game starts
void UArmyDataComponent::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!GM) return;
	
	if(GM->EconComp)
	{
		if(GetUnitDataArray().Num() > 0)
		{
			CalculateUpkeeps();
		}
	} /* Reset */
	
}

