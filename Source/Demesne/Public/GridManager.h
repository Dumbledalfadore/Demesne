// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class ATile;

UCLASS()
class DEMESNE_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	int32 GridHeight;
	UPROPERTY(EditAnywhere)
	int32 GridWidth;

	UPROPERTY(EditAnywhere)
	float TileHorOffset = 173.205078f;

	UPROPERTY(EditAnywhere)
	float TileVerOffset = 150.0f;

	UPROPERTY(EditAnywhere)
	float OddHorOffset = 86.602539;

	UPROPERTY(EditAnywhere)
	float OddVerOffset;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> GrassTile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> WaterTile;

	TArray<TArray<ATile*>> GridArray;
public:	
	// Seeds

	UPROPERTY()
	FIntPoint LandSeed;

	UPROPERTY()
	FIntPoint WaterSeed;

	// Land Generation Variables

	UPROPERTY(EditAnywhere)	// Land Amplitude		
		int LandAmp = 10;

	UPROPERTY(EditAnywhere)	// Land Frequency
		int LandFrq = 10;


	// Water Generation Variables

	UPROPERTY(EditAnywhere)	// Water Frequency
		int WaterFrq = 1;

	UPROPERTY(EditAnywhere)	// The Z position of all water tiles
		int WaterZPos = -25;

	/* Percentage chance of a Land Tile with a Z value below 0 becoming a Water Tile */
	UPROPERTY(EditAnywhere)
	float WaterOdds = 50;

	UPROPERTY(EditAnywhere)
	int EnviromentOdds = 10;

	UPROPERTY()
	float PerlinFactor = 10.0f;

	// Perlin noise generates values between -1 to 1 so the range is 2
	UPROPERTY()
	int PerlinRange = 2;
private:
	UFUNCTION()
	void InitGrid();
	UFUNCTION()
	void SpawnBuildings();

	UPROPERTY()
	bool foundLand = false;

};
