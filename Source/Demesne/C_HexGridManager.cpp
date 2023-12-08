// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HexGridManager.h"
#include "C_HexTile.h"

// Sets default values
AC_HexGridManager::AC_HexGridManager()
{
	PrimaryActorTick.bCanEverTick = false;



}

// Called when the game starts or when spawned
void AC_HexGridManager::BeginPlay()
{
	Super::BeginPlay();
	InitGrid();
}

void AC_HexGridManager::InitGrid()
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
			const bool oddrow = y % 2 == 1;
			
			const float xPos = oddrow ? (x * TileHorOffset) + OddHorOffset : x * TileHorOffset;
			const float yPos = oddrow ? (y * TileVerOffset) + OddVerOffset : y * TileVerOffset;

			TSubclassOf <AC_HexTile> tileToSpawn = GrassTile;

			AC_HexTile* newTile = GetWorld()->SpawnActor<AC_HexTile>(tileToSpawn, FVector(xPos, yPos, 0.0f), FRotator::ZeroRotator);
			newTile->tileIndex = FIntPoint(x, y);
			GridArray[x][y] = newTile;
		}
	}
}



