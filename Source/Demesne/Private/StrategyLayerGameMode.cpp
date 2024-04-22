// Fill out your copyright notice in the Description page of Project Settings.
#include "StrategyLayerGameMode.h"
#include "EconomyComponent.h"
#include "GridManager.h"
#include "TurnManager.h"
#include "AI/StrategyAIPawn.h"
#include "Military/ArmyManager.h"
#include "Settlements/SettlementManager.h"
#include "Utils/Pathfinder.h"


//Econ stuff is here save them to a slot before starting a battle


AStrategyLayerGameMode::AStrategyLayerGameMode()
{
	EconComp = CreateDefaultSubobject<UEconomyComponent>(TEXT("Economy Component"));
	EconComp->SetupAttachment(RootComponent);
	NumberofPlayers = 4;
	
	if(EconComp->IsValidLowLevel())
	{
		//Econ Comp doesn't initialise values correctly if called here so they are now done in the component itself
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("EconComp Invalid!!!!"));
	}
}

void AStrategyLayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	/* Spawn Managers */
	if(SettlementManagerClass) SettlementManager = GetWorld()->SpawnActor<ASettlementManager>(SettlementManagerClass);
	if(GridManagerClass) GridManager = GetWorld()->SpawnActor<AGridManager>(GridManagerClass);
	if(TurnManagerClass) TurnManager = GetWorld()->SpawnActor<ATurnManager>(TurnManagerClass);
	if(ArmyManagerClass) ArmyManager = GetWorld()->SpawnActor<AArmyManager>(ArmyManagerClass);
	if(PathFinderClass) PathFinder = GetWorld()->SpawnActor<APathfinder>(PathFinderClass);
	if(EconComp) EconComp->InitTurnManger();
	if(StrategyAIClass)
	{
		
		for(int i = 1; i <= NumberofPlayers -1; i++)
		{
			AIPawn = GetWorld()->SpawnActor<AStrategyAIPawn>(StrategyAIClass);
			AIPawn->PlayerID = i;
		}
	}//TODO: Spawn (NumberOfPlayers - 1) amount of AI once Behaviour Tree is complete
}

int AStrategyLayerGameMode::GetCurrentTurn()
{
	return mTurn;
}

int AStrategyLayerGameMode::GetMaximumTurn()
{
	return mMaxTurns;
}

int AStrategyLayerGameMode::GetCurrentTurnNumber()
{
	return mTurn;
}

void AStrategyLayerGameMode::IncrementTurnNumber()
{
	//UE_LOG(LogTemp,Warning,TEXT("Increment Turn"));
	++mTurn;
	//UE_LOG(LogTemp,Log,TEXT("Turn: %d"), mTurn);
}
