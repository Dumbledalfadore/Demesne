// Fill out your copyright notice in the Description page of Project Settings.


#include "EconHelper.h"

EconHelper::EconHelper()
{
}

EconHelper::~EconHelper()
{
}

float EconHelper::AddInterest(float balance, float rate, int turns)
{
	//interest = amount * rate * time
	//calculate interest
	/*balance = loan amount
	 * rate = interest rate in percentage
	 * turns =  how many turns since loan was taken
	 */
	return balance * rate * turns;//ignoring time for now, until the turn system is complete
	
	
}

float EconHelper::CalculateRevenue(float income, float outcome)
{
	//income per turn - upkeep per turn 
	return income - outcome;
}

bool EconHelper::isInDebt(float revenue)
{
	//if income is higher than upkeep, player/AI is not in debt, otherwise set debt to true
	if(revenue >= 0.00f){return false;}
	else{return true;}
}

bool EconHelper::isInDebt(float income, float outcome)
{
	//combines above with revenue calculation function
	
	return (CalculateRevenue(income,outcome) < 0.f ? true : false); 
}
//=====PLACEHOLDERS -SUBJECT TO BALANCE CHANGES====
float EconHelper::CalculateTotalUnitUpkeep(int UnitNum,int TurnNum, int MaxTurn, float Multiplier)
{
	/*
	 * Final Cost  = (N * B(1 + PF * M)/100)
	 * N = Number of Units
	 * B = BaseUnit Cost
	 * PF= Progress Factor
	 * M = Multiplier
	 */
	float ProgressFactor = TurnNum / MaxTurn; // Get how far along the game is until the last turn
	return (UnitNum * 50.f * (1+ ProgressFactor * Multiplier)/100);
	
}

float EconHelper::CalculateFoodForNextLevel(int Pop)
{
	//This is just a simple equation based on Civilisation V until the food mechanics is more fleshed out
	return pow(15.f + (8.f * (Pop -1)) + (Pop - 1),1.5f);
	
}

float EconHelper::CalculateGrowthForNextLevel(int Pop, float Multiplier)
{
	int PrevPopLevel = Pop - 1;
	int HalfPop = Pop * 0.5;
	
	int GrowthReq =  (BaseGrowthRate * PrevPopLevel) + (HalfPop * Multiplier);
	return GrowthReq;
	
}

float EconHelper::CalculateStarvation(int Pop, int Food, float Multiplier)
{
	/*
	 * S = (B*P) + (F*M)
	 * S = Starvation
	 * B = Base Decay
	 * P = Population Level
	 * F = Food
	 * M = Multiplier
	 * if S < 0 then there is less food than required 
	 * 
	 */
	return (-10.f * Pop) + (Food * Multiplier);
}

bool EconHelper::bIsStarving(int Pop, int Food, float Multiplier)
{
	return (CalculateStarvation(Pop,Food,Multiplier) < 0.f ? true : false);
}

bool EconHelper::bGoldIsGreaterThan(float AmountToCompare, TArray<float> Gold, float Revenue, int PlayerID)
{
	//Revenue is Income + Outcome, Gotten via Functions above
	if(AmountToCompare >= (Gold[PlayerID] + Revenue))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool EconHelper::bGoldIsLesserThan(float AmountToCompare, TArray<float> Gold, float Revenue, int PlayerID)
{
	if(AmountToCompare < (Gold[PlayerID] + Revenue))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EconHelper::bFoodIsGreaterThan(float AmountToCompare, TArray<float> Food, float Revenue, int PlayerID)
{
	if(AmountToCompare >= (Food[PlayerID] + Revenue))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EconHelper::bFoodIsLesserThan(float AmountToCompare, TArray<float> Food, float Revenue, int PlayerID)
{
	if(AmountToCompare < (Food[PlayerID] + Revenue))
	{
		return true;
	}
	else
	{
		return false;
	}
}
