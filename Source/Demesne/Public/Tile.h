// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

enum class ETileType : uint8;
class UStaticMeshComponent;
class APlaceHolder;
class ASettlement;

UCLASS()
class DEMESNE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(VisibleAnywhere)
	FIntPoint TileIndex;
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	ETileType TileType;

	UFUNCTION()
	void SpawnSettlement(int TeamNumber, TSubclassOf<ASettlement> SettlementToSpawn, bool IsMajor, bool IsCoastal);

	UFUNCTION()
	void SpawnFlora();

	UFUNCTION()
	void SetIsOccupied(const bool State) { IsOccupied = State; }

	UFUNCTION()
	bool GetIsOccupied() const { return IsOccupied; }

	UFUNCTION()
	void SetHasPlant(const bool State) { HasPlant = State; }
	
	UFUNCTION()
	bool GetHasPlant() const { return HasPlant; }

	UFUNCTION()
	void SetHasSettlement(const bool State)  { HasSettlement = State; }

	UFUNCTION()
	bool GetHasSettlement() const { return HasSettlement; }

	UFUNCTION()
	void SetIsLand(const bool State) { IsLand = State; }

	UFUNCTION()
	bool GetIsLand() const { return IsLand; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Tile")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(VisibleAnywhere)
	class AArmyUnit* OccupyingUnit;

	UPROPERTY(VisibleAnywhere)
	bool IsOccupied;

	UPROPERTY(VisibleAnywhere)
	bool HasPlant = false;

	UPROPERTY(VisibleAnywhere)
	bool HasSettlement = false;

	UPROPERTY(VisibleAnywhere)
	bool IsLand = false;

private:
	UPROPERTY()
	FVector PlaceholderPosition = FVector(0.0f, 0.0f, 90.0f);

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlaceHolder> FloraPlaceholder;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APlaceHolder> SettlementPlaceholder;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* OnTileSpawnLocation;
};
