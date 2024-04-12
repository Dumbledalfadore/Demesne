// Created by WarRoom Wizards ©2023


#include "GridManager.h"

#include "StrategyLayerGameMode.h"
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

	AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM)
	{
		GM->GridManager = this; /* Set the GridManager to this */
	}

	//sets the seeds & odds
	LandSeed = FIntPoint(FMath::RandRange(10000, 99999), FMath::RandRange(10000, 99999));
	WaterSeed = FIntPoint(FMath::RandRange(10000, 99999), FMath::RandRange(10000, 99999));
	WaterOdds = ((WaterOdds / 100) * PerlinRange) - (PerlinRange / 2);

	InitGrid();//executes the initialise grid function
	
	//spawns major settlements
	SettlementToSpawn = MajorSettlement;
	SpawnSettlements();


	//spawns minor settlements
	IsMajor = false;
	SettlementToSpawn = MinorSettlement;
	SpawnSettlements();

	/* Spawn camps */
	IsMajor = false;
	SettlementToSpawn = CampSettlement;
	SpawnSettlements();
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
			if (TileTypeToSpawn == 0) NewTile->SetIsLand(true);//if a land tile then set the tiles island boolean to true
		}
	}
}

void AGridManager::SpawnSettlements()
{
	int x, y;//declares the x & y

	int BuildQuantity = 0;
	switch (SettlementToSpawn.GetDefaultObject()->GetSettlementType())
	{
	case ESettlementType::Camp:
		BuildQuantity = CampSettlementCount;
		break;
	case ESettlementType::Minor:
		BuildQuantity = MinorSettlementCount;
		break;
	case ESettlementType::Major:
		BuildQuantity = MajorSettlementCount;
		break;
	}
	
	for (int i = 1; i <= BuildQuantity; i++)//loops for starting buildings
	{
		HasFoundLand = false;//sets found land to false
		do
		{
			x = FMath::RandRange(0, GridWidth - 1); //random x within grid
			y = FMath::RandRange(0, GridHeight - 1);//random y within grid

			//checks if its a land tile
			if (GridArray[x][y]->GetIsLand())
			{
				HasFoundLand = true; 
			}

			//checks if there is already  a building or plant on the tile
			if (GridArray[x][y]->GetHasSettlement() || GridArray[x][y]->GetHasPlant())
			{
				HasFoundLand = false;
			}
		} while (!HasFoundLand);//if found land break out of loop

		if (IsMajor)
		{
			GridArray[x][y]->SpawnSettlement(i, SettlementToSpawn, IsMajor, CheckIsCoastal(x, y));//use spawn building function passing in the loop index as the team number
		}
		else
		{
			GridArray[x][y]->SpawnSettlement(0, SettlementToSpawn, IsMajor, CheckIsCoastal(x, y));
		}
	}

}

bool AGridManager::CheckIsCoastal(int x, int y)
{
	/* X-1  (Top) */
	if(x-1 <= GridWidth - 1 && x-1 >= 0)
	{
		ATile*& TopTile = GridArray[x - 1][y];
		if(TopTile && !TopTile->GetIsLand()) { return true; }
	}

	/* X-1 Y+1 (Upper Left) */
	if (x-1 <= GridWidth - 1 && x-1 >= 0 && y+1 <= GridHeight - 1 && y+1 >= 0)
	{
		ATile*& ULTile = GridArray[x - 1][y + 1];
		if(ULTile && !ULTile->GetIsLand()) { return true; }
	}

	/* X-1 Y-1 (Upper Right) */
	if (x-1 <= GridWidth - 1 && x-1 >= 0 && y-1 <= GridHeight - 1 && y-1 >= 0)
	{
		ATile*& URTile = GridArray[x - 1][y - 1];
		if(URTile && !URTile->GetIsLand()) { return true; }
	}

	/* Y+1 (Lower Left) */
	if (y+1 <= GridHeight - 1 && y+1 >= 0)
	{
		ATile*& LLTile = GridArray[x][y + 1];
		if(LLTile && !LLTile->GetIsLand()) { return true; }
	}

	/* Y-1 (Lower Right) */
	if (y-1 <= GridHeight - 1 && y-1 >= 0)
	{
		ATile*& LRTile = GridArray[x][y - 1];
		if(LRTile && !LRTile->GetIsLand()) { return true; }
	}
	
	/* X+1 (Bottom) */
	if (x+1 <= GridWidth - 1 && x+1 >= 0)
	{
		ATile*& BottomTile = GridArray[x+1][y];
		if(BottomTile && !BottomTile->GetIsLand()) { return true; }
	}

	return false; /* No tiles are water, the tile is land locked */
}


