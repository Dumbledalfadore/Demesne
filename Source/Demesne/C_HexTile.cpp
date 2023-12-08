// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HexTile.h"

// Sets default values
AC_HexTile::AC_HexTile()
{
	
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	tileMesh->SetupAttachment(RootComponent);
}


