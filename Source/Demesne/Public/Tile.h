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

	UFUNCTION()
	void SetWasVisited(const bool State) { WasVisited = State; }

	UFUNCTION()
	bool GetWasVisited() const { return WasVisited; }

	UFUNCTION()
	void SetValidMoveTile(const bool State) { ValidMoveTile = State; }

	UFUNCTION()
	bool GetValidMoveTile() const { return ValidMoveTile; }

	UFUNCTION()
	void SetParentTile(ATile* Tile) { ParentTile = Tile; }

	UFUNCTION()
	ATile* GetParentTile() const { return ParentTile; }

	UFUNCTION()
	void AddNeighbour(ATile* Tile) { Neighbours.Add(Tile); }

	UFUNCTION()
	TArray<ATile*> GetNeighbours() { return Neighbours; }

	UFUNCTION()
	void SetGlobalGoal(const float Goal) { GlobalGoal = Goal; }
	
	UFUNCTION()
	float GetGlobalGoal() const { return GlobalGoal; }

	UFUNCTION()
	void SetLocalGoal(const float Goal) { LocalGoal = Goal; }
	
	UFUNCTION()
	float GetLocalGoal() const { return LocalGoal; }

	UFUNCTION()
	void SetValidMoveOverlayVisibility(const bool State) const;

	
protected:
	virtual void BeginPlay() override;
	
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

	/* Pathfinding */
	UPROPERTY()
	bool WasVisited = false;

	UPROPERTY()
	bool ValidMoveTile = false;

	UPROPERTY()
	ATile* ParentTile;

	UPROPERTY()
	TArray<ATile*> Neighbours;

	UPROPERTY()
	float GlobalGoal = INFINITY;

	UPROPERTY()
	float LocalGoal = INFINITY;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> ValidMoveOverlayClass;

	UPROPERTY()
	AStaticMeshActor* ValidMoveOverlay;

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
