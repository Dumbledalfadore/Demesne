// Created by WarRoom Wizards ©2023


#include "Military/ArmyUnit.h"

#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyDataComponent.h"
#include "Military/UnitData.h"

class AStrategyLayerGameMode;
// Sets default values
AArmyUnit::AArmyUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character Mesh"));
	//SetRootComponent(MeshComponent);

	ArmyComponent = CreateDefaultSubobject<UArmyDataComponent>(TEXT("Army Component"));

}

// Called when the game starts or when spawned
void AArmyUnit::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM && GM->TurnManager)
	{
		GM->TurnManager->OnTurnEnd.AddUniqueDynamic(this, &ThisClass::AArmyUnit::OnNextTurn);
	}

	/* Set the default */
	RemainingMovementDistance = MaxMovementDistance;

	if(ArmyComponent)
	{
		/* Add the default unit to the army */
		ArmyComponent->AddUnit(DefaultUnitData);
	}
}

void AArmyUnit::Destroyed()
{
	Super::Destroyed();
	
	if(GM && GM->TurnManager)
	{
		GM->TurnManager->OnTurnEnd.RemoveDynamic(this, &ThisClass::OnNextTurn);
	}
}

void AArmyUnit::OnNextTurn()
{
	/* Reset the movement distance each turn */
	RemainingMovementDistance = MaxMovementDistance;

	/* Remove the old values before we update with the new ones, they could be the same if there are no new units */
	if(GM->EconComp)
	{
		GM->EconComp->SubtractFoodUpkeep(GetPlayerID(), FoodUpkeep);
		GM->EconComp->SubtractGoldUpkeep(GetPlayerID(), GoldUpkeep);
	}

	/* Calculate the new values */
	ArmyComponent->CalculateUpkeeps();

	/* Set the new values */
	if(GM->EconComp)
	{
		GM->EconComp->AddFoodUpkeep(GetPlayerID(), FoodUpkeep);
		GM->EconComp->AddGoldUpkeep(GetPlayerID(), GoldUpkeep);
	}
}
