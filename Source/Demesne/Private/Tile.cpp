// Created by WarRoom Wizards ©2023


#include "Tile.h"
#include "PlaceHolder.h"
#include "Settlements/Settlement.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	
	tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	tileMesh->SetupAttachment(RootComponent);

	OnTileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("OnTile Spawn Location"));//creates the spawn location object
	OnTileSpawnLocation->SetupAttachment(tileMesh);
	OnTileSpawnLocation->SetRelativeLocation(placeHolderPos);//sets its relative position
}

//spawning buildings
void ATile::SpawnBuilding(int teamNum, TSubclassOf<ASettlement> buildingType)
{
	
	if (!hasBuilding && !hasPlant)//checks if there is already a building or plant at this position 
	{
		
		ASettlement* building = GetWorld()->SpawnActor<ASettlement>(buildingType, OnTileSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);//spawns a building at the spwn objects position
		hasBuilding = true;//sets has building to true
		ownerTeamNum = teamNum;//sets the tiles team ownership int
	}
}


//spawning plants 
void ATile::SpawnFlora()
{
	if (!hasBuilding && !hasPlant)//checks if there is already a building or plant at this position 
	{
		APlaceHolder* newPlant = GetWorld()->SpawnActor<APlaceHolder>(floraPlaceholder, OnTileSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);//spawns a plant at the spwn objects position
		hasPlant = true;//sets has plant to true 
	}
	
}

// Called when the game starts or when spawned




