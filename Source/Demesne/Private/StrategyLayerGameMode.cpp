// Fill out your copyright notice in the Description page of Project Settings.

#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"

//Econ stuff is here save them to a slot before starting a battle


void AStrategyLayerGameMode::BeginPlay()
{
	Super::BeginPlay();
	NumberofPlayers = 3;
	EconComp = CreateDefaultSubobject<UEconomyComponent>(TEXT("Economy Component"));
	for(int i = 0; i <= NumberofPlayers -1; i++)
	{
		EconComp->GoldBalance[i] = StartingGold;
		EconComp->FoodBalance[i] = StartingFood;
	}
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
	++mTurn;
}
