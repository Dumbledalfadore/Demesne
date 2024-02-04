// Created by WarRoom Wizards ©2023


#include "Tile.h"
#include "PlaceHolder.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	
	tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	tileMesh->SetupAttachment(RootComponent);

	OnTileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("OnTile Spawn Location"));
	OnTileSpawnLocation->SetupAttachment(tileMesh);
	OnTileSpawnLocation->SetRelativeLocation(placeHolderPos);
}

void ATile::SpawnBuilding(int teamNum)
{
	
	if (!hasBuilding && !hasPlant)
	{
		APlaceHolder* building = GetWorld()->SpawnActor<APlaceHolder>(buildingPlaceholder, OnTileSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);
		hasBuilding = true;
		ownerTeamNum = teamNum;
	}
}

void ATile::SpawnFlora()
{
	if (!hasBuilding && !hasPlant)
	{
		APlaceHolder* newPlant = GetWorld()->SpawnActor<APlaceHolder>(floraPlaceholder, OnTileSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);
		hasPlant = true;
	}
	
}

// Called when the game starts or when spawned




