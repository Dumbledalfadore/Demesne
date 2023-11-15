// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyLayerGameMode.h"
//Econ stuff is here for now, may have to move to a game instance depends on how we are doing battles


void AStrategyLayerGameMode::UpdateMoney(float Amount)
{
	//Adds amount to the players bank account use a negative number for upkeep
	mDucats += Amount;
}

void AStrategyLayerGameMode::UpdateFood(float Amount)
{
	//adds amount of food to player. Likewise use negative numbers for upkeep
	mFood += Amount;
}
