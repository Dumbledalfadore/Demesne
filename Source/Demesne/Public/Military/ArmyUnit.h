// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmyUnit.generated.h"

class AStrategyLayerGameMode;
class ATile;
class UUnitData;
class UArmyDataComponent;

UCLASS()
class DEMESNE_API AArmyUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmyUnit();

	UFUNCTION()
	int GetGoldCost() const {return GoldCost; }
	
	UFUNCTION()
	int GetFoodCost() const {return FoodCost; }

	UFUNCTION()
	int GetGoldUpkeep() const {return GoldUpkeep; }

	UFUNCTION()
	int GetFoodUpkeep() const {return FoodUpkeep; }
	
	UFUNCTION()
	int GetPlayerID() const { return PlayerID; }

	UFUNCTION()
	void SetPlayerID(const int ID) { PlayerID = ID; }

	UFUNCTION()
	FString GetArmyName() const { return ArmyName; }
	
	UFUNCTION()
	void SetArmyName(FString Name) { ArmyName = Name; }
	
	UFUNCTION()
	FString GetArmyDescription() { return ArmyDescription; }

	UFUNCTION()
	int32 GetMaxMovementDistance() const { return MaxMovementDistance; }

	UFUNCTION()
	void SetRemainingMovementDistance(const int32 Distance) { RemainingMovementDistance = Distance; }
	
	UFUNCTION()
	int32 GetRemainingMovementDistance() const { return RemainingMovementDistance; }

	UFUNCTION()
	UArmyDataComponent* GetArmyComponent() const { return ArmyComponent; }

	UFUNCTION()
	UUnitData* GetDefaultUnitData() { return DefaultUnitData; }

	UFUNCTION()
	TArray<UUnitData*> GetRecruitableUnits() { return RecruitableUnits; }

	UFUNCTION()
	FIntPoint GetTileIndex() const { return CurrentTileIndex; }

	UFUNCTION()
	void SetTileIndex(const FIntPoint Index) { CurrentTileIndex = Index; }

	UFUNCTION()
	AStrategyLayerGameMode* GetGM() const { return GM; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	/* ID of the controlling player */
	UPROPERTY()
	int PlayerID = -1;

	UPROPERTY(EditAnywhere)
	FString ArmyName;

	/* #OPTIONAL: A description to show on the tooltip */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ArmyDescription;

	UPROPERTY()
	FIntPoint CurrentTileIndex;

	UPROPERTY(EditAnywhere)
	int32 MaxMovementDistance;

	UPROPERTY(VisibleAnywhere)
	int32 RemainingMovementDistance;

	/* Holds data about the troops in the garrison */
	UPROPERTY(VisibleAnywhere)
	class UArmyDataComponent* ArmyComponent;

	/* The unit that 'leads' the army */
	UPROPERTY(EditAnywhere)
	class UUnitData* DefaultUnitData;

	/* The units that can be recruited for the army */
	UPROPERTY(EditAnywhere)
	TArray<class UUnitData*> RecruitableUnits;

	UFUNCTION()
	void OnNextTurn();

	/* Upkeeps */
	UPROPERTY(BlueprintReadOnly)
	float FoodUpkeep;

	UPROPERTY(BlueprintReadOnly)
	float GoldUpkeep;

	/* Costs to spawn */
	UPROPERTY(BlueprintReadOnly)
	float FoodCost;

	UPROPERTY(BlueprintReadOnly)
	float GoldCost;

	UPROPERTY()
	AStrategyLayerGameMode* GM;
};
