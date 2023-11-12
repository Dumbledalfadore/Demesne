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
	float revenue = income - outcome;
	if(revenue >= 0.00f){return false;}
	else{return true;}
}
