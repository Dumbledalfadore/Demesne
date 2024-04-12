// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pathfinder.generated.h"

class AGridManager;
class ATile;

UCLASS()
class DEMESNE_API APathfinder : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APathfinder();
	
	UFUNCTION()
	bool FindPath(ATile* StartTile, ATile* EndTile, bool HasMaxDistance, int32 MaxDistance);

	UFUNCTION()
	void FindValidTiles(ATile* StartTile, int32 MaxDistance);

	UFUNCTION()
	void UnitDeselected();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	AGridManager* GridManager;
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid")
	int32 GridWidth;

	UPROPERTY(BlueprintReadOnly, Category = "Grid")
	int32 GridHeight;

	UFUNCTION()
	bool Solve_AStar(bool HasMaxDistance, int32 MaxDistance);

	UFUNCTION()
	static float Distance(const ATile* StartTile, const ATile* EndTile);
	
	UFUNCTION()
	bool CreatePathfindGrid();

	UPROPERTY()
	ATile* TileStart;

	UPROPERTY()
	ATile* TileEnd;

	UPROPERTY()
	ATile* PreviousTileStart;

	UPROPERTY()
	ATile* PreviousTileEnd;


	UFUNCTION()
	TArray<ATile*> RetracePath() const; /* Returns an array of tiles forming a path, useful for AI */

	UFUNCTION()
	void EnableMoveTiles();

	UFUNCTION()
	void DisableMoveTiles();
	
	UPROPERTY()
	TArray<ATile*> MoveTiles; /* Valid tiles to move to */
};
