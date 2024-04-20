// Created by WarRoom Wizards ©2023


#include "Utils/Pathfinder.h"

#include "GridManager.h"
#include "StrategyLayerGameMode.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APathfinder::APathfinder()
{
}

// Called when the game starts or when spawned
void APathfinder::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("PATHFINDER"));
	AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!GM) return;
	
	GridManager = GM->GridManager;

	if (GridManager)
	{
		GridWidth = GridManager->GetGridWidth();
		GridHeight = GridManager->GetGridHeight();
		//UE_LOG(LogTemp, Warning, TEXT("GridWidth: %d GridHeight: %d"), GridWidth, GridHeight);

		CreatePathfindGrid();
	}
}

/* Sets start and end tiles then calls the pathfinding so we dont try to call it without the tiles set */
bool APathfinder::FindPath(ATile* StartTile, ATile* EndTile, bool HasMaxDistance, int MaxDistance)
{
	TileStart = StartTile;
	TileEnd = EndTile;

	return Solve_AStar(HasMaxDistance, MaxDistance);
}

bool APathfinder::CreatePathfindGrid()
{
	/* Initialise path info */
	FIntPoint Index = {0, 0};
	FIntPoint NeighbourIndex = {0, 0};
	for (int x = 0; x < GridWidth; x++)
	{
		Index.X = x;
		for (int y = 0; y < GridHeight; y++)
		{
			Index.Y = y;
			ATile* Tile = GridManager->GetTile(Index);
			//GridManager->GetTile(Index)->bObstacle = false;
			Tile->SetWasVisited(false);
			Tile->SetParentTile(nullptr);

			/* Create Neighbour Connections  */
			/* Top Tile*/
			if (x-1 <= GridWidth - 1 && x-1 >= 0)
			{
				NeighbourIndex = {x - 1, y};
				Tile->AddNeighbour(GridManager->GetTile(NeighbourIndex));
			}

			/* Upper Left */
			if (x-1 <= GridWidth - 1 && x-1 >= 0 && y+1 <= GridHeight - 1 && y+1 >= 0)
			{
				NeighbourIndex = {x - 1, y + 1};
				Tile->AddNeighbour(GridManager->GetTile(NeighbourIndex));
			}

			/* Upper Right */
			if (x-1 <= GridWidth - 1 && x-1 >= 0 && y-1 <= GridHeight - 1 && y-1 >= 0)
			{
				NeighbourIndex = {x - 1, y - 1};
				Tile->AddNeighbour(GridManager->GetTile(NeighbourIndex));
			}

			/* Lower Left */
			if (y+1 <= GridHeight - 1 && y+1 >= 0)
			{
				NeighbourIndex = {x, y + 1};
				Tile->AddNeighbour(GridManager->GetTile(NeighbourIndex));
			}

			/* Lower Right  */
			if (y-1 <= GridHeight - 1 && y-1 >= 0)
			{
				NeighbourIndex = {x, y - 1};
				Tile->AddNeighbour(GridManager->GetTile(NeighbourIndex));
			}

			/* Bottom */
			if (x+1 <= GridWidth - 1 && x+1 >= 0)
			{
				NeighbourIndex = {x + 1, y};
				Tile->AddNeighbour(GridManager->GetTile(NeighbourIndex));
			}
		}
	}
	
	return true;
}


bool APathfinder::Solve_AStar(const bool HasMaxDistance, const int32 MaxDistance)
{
	//UE_LOG(LogTemp, Warning, TEXT("SolveAStar"));
	/* Reset pathfinding graph - default all tiles */
	FIntPoint Index = {0, 0};
	for (int x = 0; x < GridWidth; x++)
	{
		Index.X = x;
		for (int y = 0; y < GridHeight; y++)
		{
			Index.Y = y;
			ATile* Tile = GridManager->GetTile(Index);
			
			Tile->SetWasVisited(false);
			Tile->SetGlobalGoal(INFINITY);
			Tile->SetLocalGoal(INFINITY);
			Tile->SetParentTile(nullptr);
		}
	}

	/* Setup starting conditions */
	ATile* TileCurrent = TileStart;
	TileCurrent->SetLocalGoal(0.0f);
	TileCurrent->SetGlobalGoal(Distance(TileStart, TileEnd));

	if (HasMaxDistance)
	{
		if (TileCurrent->GetGlobalGoal() > MaxDistance * 100)
		{
			return false;
		}
	}

	/* Add start node to not tested list, this will make sure it gets tested.
	* As the algorithm progresses, newly discovered nodes get added to this
	* list, and will then be tested later */

	TArray<ATile*> OpenSet; /* Tiles not tested yet */ 
	TArray<ATile*> ClosedSet; /* Tiles that have been tested */

	OpenSet.Add(TileStart);

	/* If the not tested list contains tiles, there may be better paths
	* which have not yet been explored. However, we will also stop 
	* searching when we reach the target - there may be better
	* paths but we just want the shortest. */
	while (OpenSet.Num() != 0 && TileCurrent != TileEnd) /* Find absolutely shortest path // && TileCurrent != TileEnd) */
	{
		/* Sort untested tiles by global goal, so lowest is top of the list */
		OpenSet.Sort([](const ATile& LHS, const ATile& RHS)
			{
				return LHS.GetGlobalGoal() > RHS.GetGlobalGoal();
			});

		/* Top of OpenSet is potentially the lowest distance node.
		 * Our list may also contain tiles that have been visited, so get rid of them */
		while (OpenSet.Num() != 0 && OpenSet.Top()->GetWasVisited())
		{
			ATile* TestedTile = OpenSet.Top();
			OpenSet.Pop();
			ClosedSet.Push(TestedTile);
		}

		/* Abort because we have no more tiles left */
		if (OpenSet.Num() == 0) break;

		TileCurrent = OpenSet.Top();
		TileCurrent->SetWasVisited(true); /* We only visit a tile once */


		/* Check each of this node's neighbours */
		for (auto TileNeighbour : TileCurrent->GetNeighbours())
		{
			/* But only if the neighbour is not visited and is 
			* not an obstacle, add it to OpenSet array */
			if (!TileNeighbour->GetWasVisited() /* && !TileNeighbour->bObstacle */)
			{
				OpenSet.Push(TileNeighbour);
			}

			/* Calculate the neighbours potential lowest parent distance */
			float PossiblyLowerGoal = TileCurrent->GetLocalGoal() + Distance(TileCurrent, TileNeighbour);

			/* If choosing to path through this tile is a lower distance than what 
			* the neighbour currently has set, update the neighbour to use this tile
			* as the path source, and set its distance scores as necessary */
			if (PossiblyLowerGoal < TileNeighbour->GetLocalGoal())
			{
				TileNeighbour->SetParentTile(TileCurrent);
				TileNeighbour->SetLocalGoal(PossiblyLowerGoal);

				/* The best path length to the neighbour being tested has changed, so
				* update the neighbour's score */
				TileNeighbour->SetGlobalGoal(TileNeighbour->GetLocalGoal() + Distance(TileNeighbour, TileEnd));
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("Local Goal: %f || Global Goal: %f"), TileCurrent->GetLocalGoal(), TileCurrent->GetGlobalGoal());
	}
	return true;
}

/* Calc distance between the two tiles */
float APathfinder::Distance(const ATile* StartTile, const ATile* EndTile)
{
	const FVector ALoc = FVector(StartTile->TileIndex.X, StartTile->TileIndex.Y, 0);
	const FVector BLoc = FVector(EndTile->TileIndex.X, EndTile->TileIndex.Y, 0);

	const FVector Loc = ALoc - BLoc;

	return UKismetMathLibrary::Abs(Loc.X) + UKismetMathLibrary::Abs(Loc.Y); /* Changed from using Z so we get whole numbers which is the equivalent to the amount of moves when divided by 100 */
}

/* Finds the valid tiles around the unit within range of the MaxTileMovement of the Unit */
void APathfinder::FindValidTiles(ATile* StartTile, const int32 MaxDistance)
{
	//UE_LOG(LogTemp, Warning, TEXT("FindValidTiles: %i"), MaxDistance);

	DisableMoveTiles(); /* Disable the tile overlays */

	FIntPoint Index = {0, 0};
	for (int x = 0; x < GridWidth; x++)
	{
		Index.X = x;
		for (int y = 0; y < GridHeight; y++)
		{
			Index.Y = y;
			ATile* Tile = GridManager->GetTile(Index);
			Tile->SetValidMoveTile(false);
			const float DistanceCalc = Distance(StartTile, Tile);
			if (DistanceCalc <= MaxDistance && DistanceCalc != 0 && !Tile->GetHasPlant())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distance);
				if (FindPath(StartTile, Tile, true, MaxDistance)) /* Check to see if the location can be reached or not */
				{
					//UE_LOG(LogTemp, Warning, TEXT("ValidTileFound %f"), DistanceCalc);
					Tile->SetValidMoveTile(true);
					MoveTiles.Push(Tile);
				}
			} //* We dont want to see the paths from this function so mark the tiles as not visited */
			Tile->SetWasVisited(false);
		}
	}

	EnableMoveTiles(); /* Enable the tile overlays */

	//UE_LOG(LogTemp, Warning, TEXT("FindValidTiles Completed"));
}

void APathfinder::UnitDeselected()
{
	DisableMoveTiles();
}

/* To use this you have a couple of options
*  1) - Call SetPathStart on the tile of the unit, then SetPathEnd on the tile you want to move to
*     - and as long as both the start and end are set, it will automatically call FindPath()
*
*  2) - A little more simpler would be to call FindPath() As this will set the start and end and then automatically
*       call the Solve_AStar(); function
*
*   After you complete one of those, you can then call RetracePath which will return the path as an array of tiles
*/

/* Returns a path for the AI to take */
TArray<ATile*> APathfinder::RetracePath() const
{
	TArray<ATile*> Path;
	if (TileStart != nullptr && TileEnd != nullptr)
	{
		ATile* Tile = TileEnd;
		while (Tile->GetParentTile() != nullptr && Tile->GetWasVisited())
		{
			Path.Push(Tile);
			Tile = Tile->GetParentTile();
		}
	}

	return Path;
}

void APathfinder::EnableMoveTiles()
{
	//UE_LOG(LogTemp, Warning, TEXT("SpawnMoveTiles"));
	if (MoveTiles.Num() > 0)
	{
		for (auto& Tile : MoveTiles)
		{
			Tile->SetValidMoveOverlayVisibility(true);
		}
	}
}

void APathfinder::DisableMoveTiles()
{
	//UE_LOG(LogTemp, Warning, TEXT("RemoveMoveTiles"));

	if (MoveTiles.Num() > 0)
	{
 		for (auto& Tile : MoveTiles)
		{
 			Tile->SetValidMoveOverlayVisibility(false);
		}

		MoveTiles.Empty();
	}
}
