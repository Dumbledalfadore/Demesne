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

private:
	//currency variables
	float mDucats = 0.0f;
	float mDebt = 0.0f;
	float mFood = 0.0f;
	float mInterestRate = 5.00f;
public:
	UFUNCTION(BlueprintCallable,Category = "Economy")
	void UpdateMoney(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Economy")
	void UpdateFood(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Economy")
	float GetMoney(){return mDucats;};
	UFUNCTION(BlueprintCallable, Category = "Economy")
	float GetFood(){return mFood;};
	UFUNCTION(BlueprintCallable, Category = "Economy")
	float GetInterestRate(){return mInterestRate;};
	

};
