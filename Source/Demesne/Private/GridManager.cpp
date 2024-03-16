// Created by WarRoom Wizards ©2023


#include "GridManager.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Settlements/Settlement.h"
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
	
	//spawns major settlements
	SetToSpawn = MajorSet;
	SpawnBuildings();


	//spawns minor settlements
	IsMajor = false;
	SetToSpawn = MinorSet;
	SpawnBuildings();

	/* Spawn camps */
	IsMajor = false;
	SetToSpawn = CampSet;
	SpawnBuildings();
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
			const bool IsOddRow = y % 2 == 1;

			const float xPos = IsOddRow ? (x * TileHorOffset) + OddHorOffset : x * TileHorOffset;
			const float yPos = IsOddRow ? (y * TileVerOffset) + OddVerOffset : y * TileVerOffset;

			//--------switch case to assign tiletype and the grass tiles z position
			TSubclassOf <ATile> TileToSpawn;
			switch (TileTypeToSpawn)
			{
			case 0: 
				TileToSpawn = GrassTile; zPos = FMath::Clamp(LandValue, -20.0f, 225.0f); 
				break;
			case 1:
				TileToSpawn = WaterTile; break;
			}

			//--------spawns the tile actor
			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileToSpawn, FVector(xPos, yPos, zPos), FRotator::ZeroRotator);
			NewTile->TileIndex = FIntPoint(x, y);// assings the tile an index value
			NewTile->TileType = (TileToSpawn == GrassTile) ? ETileType::Grass : ETileType::Water;
			GridArray[x][y] = NewTile;//adds the new tile to the array

			bool SpawnFlora = FMath::RandRange(0, EnvironmentOdds) == EnvironmentOdds && TileTypeToSpawn == 0;
			if (SpawnFlora) NewTile->SpawnFlora();//uses the spawn flora function 
			if (TileTypeToSpawn == 0) NewTile->IsLand = true;//if a land tile then set the tiles island boolean to true
		}
	}
}

void AGridManager::SpawnBuildings()
{
	int x, y;//declares the x & y
	int BuildQuantity = IsMajor ? MajorSettlementCount : MinorSettlementCount;
	for (int i = 1; i <= BuildQuantity; i++)//loops for starting buildings
	{
		HasFoundLand = false;//sets found land to false
		do
		{
			x = FMath::RandRange(0, GridWidth); //random x within grid
			y = FMath::RandRange(0, GridHeight);//random y within grid

			//checks if its a land tile
			if (GridArray[x][y]->IsLand)
			{
				HasFoundLand = true; 
			}

			//checks if there is already  a building or plant on the tile
			if (GridArray[x][y]->HasBuilding || GridArray[x][y]->HasPlant)
			{
				HasFoundLand = false;
			}
		} while (!HasFoundLand);//if found land break out of loop

		if (IsMajor)GridArray[x][y]->SpawnBuilding(i, SetToSpawn, IsMajor);//use spawn building function passing in the loop index as the team number
		else GridArray[x][y]->SpawnBuilding(0, SetToSpawn, IsMajor);
	}

}


