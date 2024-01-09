// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEMESNE_API EconHelper
{
public:
	EconHelper();
	~EconHelper();
	static float AddInterest(float balance, float rate, int turns);
	static float CalculateRevenue(float income, float outcome);
	static bool isInDebt(float revenue);
	static bool isInDebt(float income, float outcome);
	static float CalculateTotalUnitUpkeep(int UnitNum,int TurnNum, int MaxTurn,float Multiplier);
	static float CalculateFoodForNextLevel(int Pop);
	static float CalculateStarvation(int Pop, int Food, float Multiplier);
	static bool bIsStarving(int Pop, int Food, float Multiplier);
	
};
