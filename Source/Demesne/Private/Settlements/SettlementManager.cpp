// Created by WarRoom Wizards ©2023


#include "Settlements/SettlementManager.h"

#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Settlements/Settlement.h"

// Sets default values
ASettlementManager::ASettlementManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

TArray<ASettlement*> ASettlementManager::GetSettlementsByID(const int PlayerID)
{
	if(Settlements.Num() == 0) return Settlements; /* Shouldn't even happen */

	TArray<ASettlement*> Array;

	for(auto Settlement : Settlements)
	{
		/* Check for matching ID */
		if(Settlement->GetPlayerID() == PlayerID)
		{
			Array.Add(Settlement);
		}
	}

	return Array;
}

void ASettlementManager::AddSettlement(ASettlement* Settlement)
{
	if(Settlement)
	{
		Settlements.Add(Settlement);
	}
}

void ASettlementManager::HandoverSettlement(ASettlement* Settlement, const int FromPlayerID, const int ToPlayerID)
{
	if(Settlement)
	{
		GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if(!GM || !GM->EconComp) { UE_LOG(LogTemp, Warning, TEXT("!GM || !GM->EconComp")); return; }
	
		/* Remove the income/upkeep from the old owner
		 * Default owner id is -1 so we don't want to try take any funds from it, it doesn't exist */
		if(FromPlayerID >= 0)
		{
			GM->EconComp->SubtractGoldIncome(FromPlayerID, Settlement->GetLocalGoldIncome());
			GM->EconComp->SubtractFoodIncome(FromPlayerID, Settlement->GetLocalFoodIncome());
			GM->EconComp->SubtractGoldUpkeep(FromPlayerID, Settlement->GetLocalGoldUpkeep());
			GM->EconComp->SubtractFoodUpkeep(FromPlayerID, Settlement->GetLocalFoodUpkeep());
		}
		
		Settlement->SetPlayerID(ToPlayerID);
		//UE_LOG(LogTemp, Warning, TEXT("SET PLAYER ID TO: %i"), ToPlayerID);
		/* New owner won't get any previous buildings/upgrades and has to start fresh */
		Settlement->ResetSettlement();
	}
}

// Called when the game starts or when spawned
void ASettlementManager::BeginPlay()
{
	Super::BeginPlay();
}

