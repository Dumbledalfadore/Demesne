// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StrategyLayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API AStrategyLayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//currency variables
	float Ducats = 0.0f;
	float Food = 0.0f;
	float BaseInterestRate = 5.00f;
	

};
