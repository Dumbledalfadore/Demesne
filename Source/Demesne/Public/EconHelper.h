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
	//================Econ Functions
	static float AddInterest(float balance, float rate, int turns);
	static float CalclulateLoanAmount(float base,int turn);
	static float CalculateRevenue(float income, float outcome);
	static bool isInDebt(float revenue);
	static bool isInDebt(float income, float outcome);
	static float CalculateTotalUnitUpkeep(int UnitNum,int TurnNum, int MaxTurn,float Multiplier);
	static float CalculateFoodForNextLevel(int Pop);
	static float CalculateGrowthForNextLevel(int Pop, float Multiplier = 1);
	static float CalculateStarvation(int Pop, int Food, float Multiplier);
	static bool bIsStarving(int Pop, int Food, float Multiplier);
	static bool bGoldIsGreaterThan(float AmountToCompare, TArray<float> Gold,float Revenue, int PlayerID);
	static bool bGoldIsLesserThan(float AmountToCompare, TArray<float> Gold,float Revenue, int PlayerID);
	static bool bFoodIsGreaterThan(float AmountToCompare, TArray<float> Food,float Revenue, int PlayerID);
	static bool bFoodIsLesserThan(float AmountToCompare, TArray<float> Food,float Revenue, int PlayerID);
	//====================Variables=============
	static const int BaseGrowthRate = 50.f;
};
