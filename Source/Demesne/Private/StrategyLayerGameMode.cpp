// Fill out your copyright notice in the Description page of Project Settings.
#include "StrategyLayerGameMode.h"
#include "EconomyComponent.h"


//Econ stuff is here save them to a slot before starting a battle


AStrategyLayerGameMode::AStrategyLayerGameMode()
{
	EconComp = CreateDefaultSubobject<UEconomyComponent>(TEXT("Economy Component"));
	EconComp->SetupAttachment(RootComponent);
	NumberofPlayers = 3;

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
	UE_LOG(LogTemp,Warning,TEXT("Increment Turn"));
	++mTurn;
	UE_LOG(LogTemp,Log,TEXT("Turn: %d"), mTurn);
}
