// Created by WarRoom Wizards ©2023


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	
	tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	tileMesh->SetupAttachment(RootComponent);

	
}

void ATile::SpawnBuilding()
{
	

}

// Called when the game starts or when spawned




