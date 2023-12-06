// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyLayerGameMode.h"

//Econ stuff is here save them to a slot before starting a battle


void AStrategyLayerGameMode::UpdateMoney(float Amount)
{
	//Adds amount to the players bank account use a negative number for upkeep
	Gold += Amount;
}

void AStrategyLayerGameMode::UpdateFood(float Amount)
{
	//adds amount of food to player. Likewise use negative numbers for upkeep
	Food += Amount;
}




int AStrategyLayerGameMode::GetCurrentTurnNumber()
{
	return mTurn;
}

void AStrategyLayerGameMode::IncrementTurnNumber()
{
	++mTurn;
}
