// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DemenseStructAndEnums.h"
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

private:
	//currency variables

	float mGold;
	float mFood;
	int mTurn;
	int mMaxTurns;


	
public:
	UFUNCTION(BlueprintCallable,Category = "Economy")
	void UpdateMoney(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Economy")
	void UpdateFood(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Economy")
	float GetFood();
	UFUNCTION(BlueprintCallable, Category = "Economy")
	int GetCurrentTurn();
	UFUNCTION(BlueprintCallable, Category = "Economy")
	int GetMaximumTurn();
	UFUNCTION(BlueprintCallable, Category = "Economy")
	float GetGold();
	UFUNCTION(BlueprintCallable, Category = "Turns")
	int GetCurrentTurnNumber();
	UFUNCTION(BlueprintCallable, Category = "Turns")
	void IncrementTurnNumber();

	

};
