// Fill out your copyright notice in the Description page of Project Settings.
#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"


//Econ stuff is here save them to a slot before starting a battle


AStrategyLayerGameMode::AStrategyLayerGameMode()
{
	EconComp = CreateDefaultSubobject<UEconomyComponent>(TEXT("Economy Component"));
	EconComp->SetupAttachment(RootComponent);
	NumberofPlayers = 3;

	if(EconComp->IsValidLowLevel())
	{
		for(int i = 0; i <= NumberofPlayers -1; i++)
		{
		
			/*EconComp->ChangeFoodBalance(i,StartingFood);
			EconComp->ChangeGoldBalance(i,StartingGold);
			EconComp->SetGoldIncome(i,StartingGoldIncome);
			EconComp->SetGoldUpkeep(i,StartingGoldUpkeep);
			EconComp->SetFoodIncome(i,StartingFoodIncome);
			EconComp->SetFoodUpkeep(i,StartingFoodUpkeep);*/
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("EconComp Invalid!!!!"));
	}
}

void AStrategyLayerGameMode::BeginPlay()
{
	Super::BeginPlay();

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
