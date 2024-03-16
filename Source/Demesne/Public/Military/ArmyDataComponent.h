// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmyDataComponent.generated.h"


class UUnitData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMESNE_API UArmyDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArmyDataComponent();

	/* Check if we can add any more units or not */
	UFUNCTION()
	bool CheckCanAddUnit();

	/* Adds a new unit to CurrentUnitData and it's health in CurrentUnitHealth */
	UFUNCTION()
	void AddUnit(UUnitData* UnitData);

	/* Removes all units of an array -- Useful for constructing buildings */ 
	UFUNCTION()
	void AddUnits(TArray<UUnitData*> Units);

	UFUNCTION()
	void SetMaxUnitCount(const int Count) { MaxUnitsInArmy = Count; }

	UFUNCTION()
	float GetUnitHealth(const int Index);

	UFUNCTION()
	bool CheckUnitIsAlive(const int Index);

	/* Removes a specific unit */
	UFUNCTION()
	void RemoveUnitAtIndex(const int Index);

	/* Removes all dead unit */
	UFUNCTION()
	void RemoveDeadUnits();

	/* Removes any single unit -- Useful for deconstructing buildings */
	UFUNCTION()
	void RemoveUnit(UUnitData* UnitData);

	/* Removes all units of an array -- Useful for deconstructing buildings */ 
	UFUNCTION()
	void RemoveUnits(TArray<UUnitData*> Units);

	UFUNCTION()
	void ClearUnits();

	/* Adds up the total food upkeep of the units, not used for settlement garrison as the cost is part of the building */
	UFUNCTION()
	float CalculateTotalFoodUpkeep();

	/* Same as above but for gold */
	UFUNCTION()
	float CalculateTotalGoldUpkeep();

	UFUNCTION()
	float GetFoodUpkeep() const { return FoodUpkeep; }
	
	UFUNCTION()
	float GetGoldUpkeep() const { return GoldUpkeep; }

	UFUNCTION()
	void CalculateUpkeeps();

	UFUNCTION()
	TArray<UUnitData*> GetUnitDataArray(){ return CurrentUnitData; }

	UFUNCTION()
	TArray<float> GetUnitHealthDataArray(){ return CurrentUnitHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Array of units, contains all data about the unit */
	UPROPERTY(VisibleAnywhere)
	TArray<UUnitData*> CurrentUnitData;

	/* Current health of the unit listed above */
	UPROPERTY(VisibleAnywhere)
	TArray<float> CurrentUnitHealth;

	/* Maximum amount of units this army can contain */
	UPROPERTY(EditAnywhere)
	int MaxUnitsInArmy;

	UPROPERTY(VisibleAnywhere)
	float FoodUpkeep;

	UPROPERTY(VisibleAnywhere)
	float GoldUpkeep;
};
