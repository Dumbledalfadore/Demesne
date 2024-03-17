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
	UE_LOG(LogTemp, Warning, TEXT("SPAWN ARMY"));
	if(UnitToSpawn && GM)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPAWN ARMY1"));
		if(ATile* TileToSpawnAt = GM->GM->GetTile(TileIndex))
		{
			AArmyUnit* Army = GetWorld()->SpawnActor<AArmyUnit>(UnitToSpawn, TileToSpawnAt->GetActorLocation(), TileToSpawnAt->GetActorRotation());
			Army->SetPlayerID(PlayerID);
			ActiveArmies.Add(PlayerID, Army);
			UE_LOG(LogTemp, Warning, TEXT("SPAWN ARMY2"));
		}
	}
}

// Called when the game starts or when spawned
void AArmyManager::BeginPlay()
{
	Super::BeginPlay();
	
}


