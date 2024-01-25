// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingData.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

struct FBuildingData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationDelegate, FString, Notification);

UCLASS()
class DEMESNE_API ASettlement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASettlement();

	UFUNCTION()
	void ResetSettlement();

	FOnNotificationDelegate OnNotification;
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	class ATurnManager* TurnManagerRef;
	/* Getters */
	UFUNCTION()
	int GetOwnerID() const { return PlayerID; }

	UFUNCTION()
	int GetPopulation() const { return SettlementPopulation; }
	
	UFUNCTION()
	FString GetSettlementName() const { return SettlementName; }

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

	/* Similar to above but returns the current building tier */
	UFUNCTION()
	EBuildingTier GetBuildingTier(UBuildingData* Building);

	/* Gets the highest tier of settlement building allowed based on the settlement type */
	UFUNCTION()
	EBuildingTier GetBuildingTierCap();

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

	UFUNCTION()
	UBuildingData* GetEmptyBuilding() const { return EmptyBuilding; }

	UFUNCTION()
	float GetResourceValue(UBuildingData* Building, EResourceType Resource);
	
	UFUNCTION()
	float GetLocalResourceValue(UBuildingData* Building, ELocalResourceType Resource);

	/* Gets amount of food produced by the settlement */
	UFUNCTION()
	float GetLocalFood();

	/* Gets amount of gold produced by the settlement */
	UFUNCTION()
	float GetLocalGold();
	/* Setters */

	void SetPlayerID(const int ID) { this->PlayerID = ID; }

	void SetIsCoastal(const bool IsCoastal) { bIsCoastal = IsCoastal; } 

	/* Returns the available building cap from the settlement building currently built */
	UFUNCTION()
	void UpdateBuildingCapAvailable();
	
	/* Checks if the BuildingData already exists in CurrentBuildings */
	UFUNCTION()
	bool CheckAlreadyBuilt(UBuildingData* BuildingData);

	/* Checks if the CurrentBuildings already has a matching identifier,
	 * to prevent that building type from being built again */
	UFUNCTION()
	bool CheckMatchingIdentifier(UBuildingData* CurrentBuilding, UBuildingData* UpgradeBuilding);

	UFUNCTION()
	bool CheckCanAffordBuilding(UBuildingData* Building);

	UFUNCTION()
	bool CheckHasResource(EResourceType Resource, float Cost);

	UFUNCTION()
	bool CheckHasLocalResource(ELocalResourceType Resource, float Cost);

	/* Doesn't call CheckHasResource first, make sure to call that and check they are available before calling this. */
	UFUNCTION()
	void RemoveBuildingResources(UBuildingData* Building);
	
	UFUNCTION()
	void BuildBuilding(UBuildingData* Building, int Index);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class AStrategyLayerGameMode* GM;

	/* ID of the controlling player */
	UPROPERTY()
	int PlayerID;

	UPROPERTY()
	bool bIsCoastal;
	
	/* Called whenever its a new turn, used to collect gold from buildings etc */
	UFUNCTION()
	void OnNextTurn();

	UFUNCTION()
	void RecalculateValues();

	UPROPERTY()
	float CurrentGold;

	UPROPERTY()
	float CurrentFood;

	TArray<UBuildingData*> RemoveDuplicateBuildings(TArray<UBuildingData*> Array);

	/* Static mesh of the settlement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Mesh;
		
	/* The name of the settlement, will be displayed on UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement")
	FString SettlementName;
	
	/* The population of the settlement determines which buildings can be built.
	 * Isn't consumed when buildings are built */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	uint8 SettlementPopulation;
	
	/* The current rate at which the growth is increased per turn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float CurrentGrowthRate;

	/* The current amount of growth accumulated */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float AccumulatedGrowth;

	/* The growth required to reach the next pop level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float GrowthForNextPop;

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

	/* How many slots out of the cap we have available, based on how many slots the current town building provides (first slot)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	uint8 BuildingCapAvailable;

	/* Used to track how many buildings we currently have, rather than calling CurrentBuildings.Num() / .IsEmpty() */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	uint8 BuildingCount;

	/* Contains the default expansion slot - done as a building to make it easier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	UBuildingData* ExpandBuilding;

	/* Contains the default build slot - done as a building to make it easier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	UBuildingData* EmptyBuilding;

	/* Contains the default destruction slot - done as a building to make it easier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	UBuildingData* DeconstructBuilding;
	
	/* Contains the different possible settlement buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> SettlementBuildings;

	/* Contains the different possible farming buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> FarmBuildings;

	/* Contains the different possible military buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> MilitaryBuildings;

	/* Contains the different possible fishing buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> FishingBuildings;

	/* Contains the different possible craft buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> CraftBuildings;

	/* Contains the different possible religious buildings that can be built */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement|Buildings")
	TArray<UBuildingData*> ReligiousBuildings;

	/* Fixed array containing current buildings, buildings should be replaced at index rather than added */
	TStaticArray<UBuildingData*, 8> CurrentBuildings;

	/* Same as AvailableBuildings, but with any CurrentBuildings removed */
	UPROPERTY()
	TArray<UDataTable*> BuildableBuildings;
};
