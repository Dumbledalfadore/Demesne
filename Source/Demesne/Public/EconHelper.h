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
	
};
