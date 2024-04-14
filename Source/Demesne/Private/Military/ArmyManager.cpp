// Created by WarRoom Wizards ©2023


#include "Military/ArmyManager.h"

#include "EconomyComponent.h"
#include "GridManager.h"
#include "StrategyLayerGameMode.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyDataComponent.h"
#include "Military/ArmyUnit.h"
#include "Military/UnitData.h"

class ATile;
// Sets default values
AArmyManager::AArmyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AArmyManager::CreateArmy(const FIntPoint TileIndex, const TSubclassOf<AArmyUnit> UnitToSpawn, const int PlayerID)
{
	if(UnitToSpawn && GM)
	{
		if(const ATile* TileToSpawnAt = GM->GridManager->GetTile(TileIndex))
		{
			AArmyUnit* Army = GetWorld()->SpawnActor<AArmyUnit>(UnitToSpawn, TileToSpawnAt->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f), TileToSpawnAt->GetActorRotation());
			Army->SetPlayerID(PlayerID);
			Army->GetArmyComponent()->SetPlayerID(PlayerID);
			Army->SetTileIndex(TileToSpawnAt->TileIndex);
			ActiveArmies.Add(PlayerID, Army);

			/* Take the cost for the unit */
			if(GM->EconComp)
			{
				GM->EconComp->SubtractGold(PlayerID, UnitToSpawn.GetDefaultObject()->GetDefaultUnitData()->GetGoldCostToCreate());
				GM->EconComp->SubtractFood(PlayerID, UnitToSpawn.GetDefaultObject()->GetDefaultUnitData()->GetFoodCostToCreate());
			}
		}
	}
}

void AArmyManager::ArmyDestroyed(AArmyUnit* Army)
{
	Destroy();
}

void AArmyManager::InitiateBattle(UArmyDataComponent* Aggressor, UArmyDataComponent* Defender)
{
	const float AggressorPower = FMath::RoundHalfToZero(Aggressor->CalculatePower());
	const float DefenderPower = FMath::RoundHalfToZero(Defender->CalculatePower());

	//UE_LOG(LogTemp, Warning, TEXT("AGRESSOR: %f, DEFENDER: %f"), AggressorPower, DefenderPower);
	if(AggressorPower > DefenderPower)
	{
		OnBattleReported.Broadcast(Aggressor, Defender, "Aggressor");

		/* Temporary solution - Destroy the losing army or handover the settlement */
		Defender->ArmyDecimated(Aggressor->GetPlayerID());
	}
	else if(AggressorPower < DefenderPower)
	{
		OnBattleReported.Broadcast(Aggressor, Defender, "Defender");

		/* Temporary solution - Destroy the losing army or handover the settlement */
		Aggressor->ArmyDecimated(Defender->GetPlayerID());
	}
	else
	{
		OnBattleReported.Broadcast(Aggressor, Defender, "Stalemate");
	}
}

// Called when the game starts or when spawned
void AArmyManager::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}


