// Created by WarRoom Wizards ©2023


#include "Tile.h"
#include "PlaceHolder.h"
#include "StrategyLayerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Settlements/Settlement.h"
#include "Settlements/SettlementManager.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	TileMesh->SetupAttachment(RootComponent);

	OnTileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("OnTile Spawn Location"));//creates the spawn location object
	OnTileSpawnLocation->SetupAttachment(TileMesh);
	OnTileSpawnLocation->SetRelativeLocation(PlaceholderPosition);//sets its relative position
}

//spawning buildings
void ATile::SpawnBuilding(int TeamNumber, TSubclassOf<ASettlement> SettlementToSpawn, bool IsMajor)
{
	if (!HasBuilding && !HasPlant)//checks if there is already a building or plant at this position 
	{
		
		ASettlement* Settlement = GetWorld()->SpawnActor<ASettlement>(SettlementToSpawn, OnTileSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);//spawns a building at the spwn objects position
		HasBuilding = true;//sets has building to true
		AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsMajor)
		{
			OwnerID = TeamNumber - 1;//sets the tiles team ownership int
			if(GM && GM->SM)
			{
				GM->SM->HandoverSettlement(Settlement, Settlement->GetPlayerID(), OwnerID); /* Give it to the new owner */
			}
		}
		
		if(GM && GM->SM)
		{
			GM->SM->AddSettlement(Settlement); /* Add to array for use later */
		}
	}
}


//spawning plants 
void ATile::SpawnFlora()
{
	if (!HasBuilding && !HasPlant)//checks if there is already a building or plant at this position 
	{
		APlaceHolder* NewPlant = GetWorld()->SpawnActor<APlaceHolder>(FloraPlaceholder, OnTileSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);//spawns a plant at the spwn objects position
		HasPlant = true;//sets has plant to true 
	}
	
}

// Called when the game starts or when spawned




