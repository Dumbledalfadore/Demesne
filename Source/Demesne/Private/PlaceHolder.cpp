// Created by WarRoom Wizards ©2023


#include "PlaceHolder.h"

// Sets default values
APlaceHolder::APlaceHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	placeholderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placeholder Mesh"));
	placeholderMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlaceHolder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaceHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

