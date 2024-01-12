// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingData.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

struct FBuildingData;

UCLASS()
class DEMESNE_API ASettlement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASettlement();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Getters */
	UFUNCTION()
	FString GetSettlementName(){ return SettlementName; }

	UFUNCTION()
	uint8 GetBuildingCap() const { return BuildingCap; }

	UFUNCTION()
	uint8 GetBuildingCapAvailable() const { return BuildingCapAvailable; }

	UFUNCTION()
	uint8 GetBuildingCount() const { return BuildingCount; }

	/* Get a building at a specific index - used to upgrade buildings */
	UFUNCTION()
	UBuildingData* GetBuildingAtIndex(int Index);

	/* Gets buildings by the type */
	UFUNCTION()
	TArray<UBuildingData*> GetBuildingsByType(EBuildingType Type);

	/* Gets buildings by type and tier */
	UFUNCTION()
	TArray<UBuildingData*> GetBuildingsByTypeAndTier(EBuildingType Type, EBuildingTier Tier);

	/* Gets the next tier of the building, used to find what buildings we can upgrade to */
	UFUNCTION()
	EBuildingTier GetNextBuildingTier(UBuildingData* Building);

	/* Gets all available buildings to build, used for an empty building slot
	 * Won't return any settlement buildings
	 * Buildings will be tier 1 */
	UFUNCTION()
	TArray<UBuildingData*> GetBuildingsToBuild();

	/* Returns the upgrades for this building */
	UFUNCTION()
	TArray<UBuildingData*> GetUpgradeBuildings(UBuildingData* BuildingData);

	UFUNCTION()
	TArray<UBuildingData*> GetCurrentBuildings();

	/* Checks if the BuildingData already exists in CurrentBuildings */
	UFUNCTION()
	bool CheckAlreadyBuilt(UBuildingData* BuildingData);

	/* Checks if the CurrentBuildings already has a matching identifier,
	 * to prevent that building type from being built again */
	UFUNCTION()
	bool CheckMatchingIdentifier(UBuildingData* CurrentBuilding, UBuildingData* UpgradeBuilding);
	
	////////////////////////////////////////

	UFUNCTION()
	void BuildBuilding(UBuildingData* Building, int Index);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Called whenever its a new turn, used to collect gold from buildings etc
	 * TODO: Create delegate to link with TurnManager
	 */
	UFUNCTION()
	void OnNextTurn();

	TArray<UBuildingData*> RemoveDuplicateBuildings(TArray<UBuildingData*> Array);
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	//UBoxComponent* Collider;

	/* Static mesh of the settlement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Mesh;
		
	/* The name of the settlement, will be displayed on UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement")
	FString SettlementName;

	/* The population of the settlement determines which buildings can be built and how many building slots can be used.
	   Subtracted from when a building is demolished so the growth needed is slightly less */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	uint8 SettlementPopulation;

	/* The surplus population of the settlement - spent on buildings and not refunded when a building is demolished */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	uint8 SettlementSurplus;

	/* The current rate at which the growth is increased per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float CurrentGrowthRate;

	/* The current amount of growth accumulated */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float AccumulatedGrowth;

	/* The base amount of growth */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float BaseGrowthRequired;

	/* Determines how many building slots a settlement will have and what the maximum build tier is */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement")
	ESettlementType SettlementType;

	/* How many tiers and how many building slots this settlement will have - set by the settlement type.
	* Camp - 2 Slots
	* Minor - 4 Slots
	* Major - 8 Slots
	* Data can still be assigned to the different tier arrays however will only be read if it matches this value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	uint8 BuildingCap;

	/* How many slots out of the cap we have avilable, based on how many slots the current town building provides (first slot)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	uint8 BuildingCapAvailable;

	/* Used to track how many buildings we currently have, rather than calling CurrentBuildings.Num() / .IsEmpty() */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	uint8 BuildingCount;
	
	/* Contains the different possible settlement buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> SettlementBuildings;

	/* Contains the different possible settlement buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> FarmBuildings;

	/* Contains the different possible settlement buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> MilitaryBuildings;

	/* Fixed array containing current buildings, buildings should be replaced at index rather than added */
	TStaticArray<UBuildingData*, 8> CurrentBuildings;

	/* Same as AvailableBuildings, but with any CurrentBuildings removed */
	UPROPERTY()
	TArray<UDataTable*> BuildableBuildings;
};
