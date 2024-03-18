// Created by WarRoom Wizards ©2023


#include "Military/ArmyUnit.h"

#include "Military/ArmyDataComponent.h"

// Sets default values
AArmyUnit::AArmyUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character Mesh"));
	SetRootComponent(MeshComponent);

	ArmyComponent = CreateDefaultSubobject<UArmyDataComponent>(TEXT("Army Component"));

}

// Called when the game starts or when spawned
void AArmyUnit::BeginPlay()
{
	Super::BeginPlay();

	if(ArmyComponent)
	{
		/* Add the default unit to the army */
		ArmyComponent->AddUnit(DefaultUnitData);
	}
	
}