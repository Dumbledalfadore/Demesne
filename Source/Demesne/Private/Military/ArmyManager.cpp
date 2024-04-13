// Created by WarRoom Wizards ©2023


#include "Military/ArmyManager.h"

#include "GridManager.h"
#include "StrategyLayerGameMode.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyUnit.h"

class ATile;
// Sets default values
AArmyManager::AArmyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AArmyManager::CreateArmy(const FIntPoint TileIndex, const TSubclassOf<AArmyUnit> UnitToSpawn, const int PlayerID)
{
	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(UnitToSpawn && GM)
	{
		if(const ATile* TileToSpawnAt = GM->GridManager->GetTile(TileIndex))
		{
			AArmyUnit* Army = GetWorld()->SpawnActor<AArmyUnit>(UnitToSpawn, TileToSpawnAt->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f), TileToSpawnAt->GetActorRotation());
			Army->SetPlayerID(PlayerID);
			Army->SetTileIndex(TileToSpawnAt->TileIndex);
			ActiveArmies.Add(PlayerID, Army);
		}
	}
}

void AArmyManager::InitiateBattle(AArmyUnit* Aggressor, AArmyUnit* Defender)
{
	float AggressorPower = Aggressor->CalculatePower();
	float DefenderPower = Defender->CalculatePower();

	UE_LOG(LogTemp, Warning, TEXT("AGRESSOR: %f, DEFENDER: %f"), AggressorPower, DefenderPower);
}

// Called when the game starts or when spawned
void AArmyManager::BeginPlay()
{
	Super::BeginPlay();
	
}


