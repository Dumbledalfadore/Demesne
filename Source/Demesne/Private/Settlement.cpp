// Fill out your copyright notice in the Description page of Project Settings.


#include "Settlement.h"
#include "BuildingData.h"

// Sets default values
ASettlement::ASettlement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	//Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	//Collider->SetBoxExtent(FVector(100.0, 100.0, 100.0));
	//Collider->SetupAttachment(RootComponent);
	//Collider->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASettlement::BeginPlay()
{
	Super::BeginPlay();

	switch (SettlementType)
	{
	case ESettlementType::Camp:
		BuildingCap = 2;
		break;
	case ESettlementType::Minor:
		BuildingCap = 3;
		break;
	case ESettlementType::Major:
		BuildingCap = 5;
		break;
	default:
		break;
	}

	/* By default all settlements will have a settlement building slot, so create it here */
	if (AvailableBuildings.Num() > 0)
	{
		/* The settlement building data should always take the first slot of the available buildings so we can use [0] to access it */
		auto settlementBuildingData = AvailableBuildings[0];
		//auto settlementBuilding = settlementBuildingData->FindRow<FBuildingData>(FName("RowName"), "Tier1");

		auto index = AvailableBuildings.IndexOfByKey(settlementBuildingData);

		BuildBuilding(index);
	}

	FindAvailableBuildings();
}

// Called every frame
void ASettlement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASettlement::NextTurn()
{
}

void ASettlement::BuildBuilding(int BuildingIndex)
{
	/* We can't build anymore buildings in this settlement */
	if (BuildingCount == BuildingCap) return;

	/* Add the building to current buildings, BuildingIndex is retrieved from AvailableBuildings
	   We pass a tier value of 0 because all new buildings should start at the lowest tier
	   We can upgrade them later with BuildingUpgrade() */
	CurrentBuildings.Add(BuildingIndex, 0);
	BuildingCount++;

	UE_LOG(LogTemp, Warning, TEXT("Building Added"))
}


void ASettlement::FindAvailableBuildings()
{
	/* No buildings available */
	if (AvailableBuildings.IsEmpty()) return;

	/* We have already filled all building slots - Call FindAvailableUpgrades to modify current buildings */
	if (CurrentBuildings.Num() == BuildingCap) return;

	if (CurrentBuildings.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("No Current Buildings.")); }

	for (auto building : AvailableBuildings)
	{
		/* We already have a building of this type, but we want to continue through the loop */
		if (CurrentBuildings.Contains(AvailableBuildings.IndexOfByKey(building))) continue;
	}
}
