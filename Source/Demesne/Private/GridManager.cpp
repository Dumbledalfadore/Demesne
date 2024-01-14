// Created by WarRoom Wizards ©2023


#include "GridManager.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	//sets the seeds & odds
	LandSeed = FIntPoint(FMath::RandRange(10000, 99999), FMath::RandRange(10000, 99999));
	WaterSeed = FIntPoint(FMath::RandRange(10000, 99999), FMath::RandRange(10000, 99999));
	WaterOdds = ((WaterOdds / 100) * PerlinRange) - (PerlinRange / 2);

	InitGrid();//executes the initialise grid function
}
void AGridManager::InitGrid()
{
	
	GridArray.SetNumZeroed(GridWidth);
	for (int i = 0; i < GridArray.Num(); i++)
	{
		GridArray[i].SetNumZeroed(GridHeight);
	}


	for (int y = 0; y < GridHeight; y++)
	{
		for (int x = 0; x < GridWidth; x++)
		{

			float zPos = WaterZPos; // sets the base z pos to be the water level

			FVector2D LandVector = FVector2D(((x * LandFrq) + LandSeed.X) / PerlinFactor, ((y * LandFrq) + LandSeed.Y) / PerlinFactor);
			float LandValue = FMath::PerlinNoise2D(LandVector) * LandAmp;

			FVector2D WaterVector = FVector2D(((x * WaterFrq) + WaterSeed.X) / PerlinFactor, ((y * WaterFrq) + WaterSeed.Y) / PerlinFactor);
			float WaterValue = FMath::PerlinNoise2D(WaterVector);

			int TileTypeToSpawn = LandValue < WaterOdds && WaterValue < WaterOdds;


			//---------creates odd row x and y positions to create the hexagon grid shape
			const bool oddrow = y % 2 == 1;

			const float xPos = oddrow ? (x * TileHorOffset) + OddHorOffset : x * TileHorOffset;
			const float yPos = oddrow ? (y * TileVerOffset) + OddVerOffset : y * TileVerOffset;

			//--------switch case to assign tiletype and the grass tiles z position
			TSubclassOf <ATile> tileToSpawn;
			switch (TileTypeToSpawn)
			{
			case 0: tileToSpawn = GrassTile; zPos = FMath::Clamp(LandValue, -20.0f, 225.0f); break;
			case 1: tileToSpawn = WaterTile; break;
			}

			//--------spawns the tile actor
			ATile* newTile = GetWorld()->SpawnActor<ATile>(tileToSpawn, FVector(xPos, yPos, zPos), FRotator::ZeroRotator);
			newTile->tileIndex = FIntPoint(x, y);// assings the tile an index value
			GridArray[x][y] = newTile;//adds the new tile to the array
		}
	}
}

void AGridManager::SpawnBuildings()
{


}


