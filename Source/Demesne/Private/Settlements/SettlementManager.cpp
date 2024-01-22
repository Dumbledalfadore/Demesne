// Created by WarRoom Wizards ©2023


#include "Settlements/SettlementManager.h"

#include "Kismet/GameplayStatics.h"
#include "Settlements/Settlement.h"

// Sets default values
ASettlementManager::ASettlementManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

TArray<ASettlement*> ASettlementManager::GetSettlementsByID(int32 PlayerID)
{
	if(Settlements.Num() == 0) return Settlements; /* Shouldn't even happen */

	TArray<ASettlement*> Array;

	for(auto Settlement : Settlements)
	{
		/* Check for matching ID */
		if(Settlement->GetOwnerID() == PlayerID)
		{
			Array.Add(Settlement);
		}
	}

	return Array;
}

TArray<ASettlement*> ASettlementManager::GetAllSettlements()
{
	TArray<AActor*> ActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), ActorArray);
	TArray<ASettlement*> Array = reinterpret_cast<TArray<ASettlement*>&>(ActorArray); /* Not the best way, but it works */

	return Array;
}

void ASettlementManager::HandoverSettlement(ASettlement* Settlement, int32 ToPlayerID)
{
	if(Settlement)
	{
		/* New owner won't get any previous buildings/upgrades and has to start fresh */
		Settlement->ResetSettlement();
		Settlement->SetPlayerID(ToPlayerID);
	}
}

// Called when the game starts or when spawned
void ASettlementManager::BeginPlay()
{
	Super::BeginPlay();

	Settlements = GetAllSettlements();
}

