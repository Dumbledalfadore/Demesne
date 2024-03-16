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

	UPROPERTY(VisibleAnywhere)
	bool HasPlant = false;

	UPROPERTY(VisibleAnywhere)
	bool HasBuilding = false;

	UPROPERTY(VisibleAnywhere)
	bool IsLand = false;

	UPROPERTY(VisibleAnywhere)
	int OwnerID;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Tile")
	UStaticMeshComponent* TileMesh;

private:
	UPROPERTY()
	FVector PlaceholderPosition = FVector(0.0f, 0.0f, 90.0f);

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlaceHolder> FloraPlaceholder;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APlaceHolder> buildingPlaceholder;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* OnTileSpawnLocation;
};
