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

public:
	AStrategyLayerGameMode();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:

	int mTurn;
	int mMaxTurns;

public:

	UFUNCTION(BlueprintCallable, Category = "Economy")
	int GetCurrentTurn();
	UFUNCTION(BlueprintCallable, Category = "Economy")
	int GetMaximumTurn();
	UPROPERTY(BlueprintReadWrite)
	class UEconomyComponent* EconComp;
	//Amount of Gold and Food that Players and AI start with
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Economy")
	float StartingGold;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Economy")
	float StartingGoldIncome;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Economy")
	float StartingGoldUpkeep;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Economy")
	float StartingFood;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Economy")
	float StartingFoodIncome;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Economy")
	float StartingFoodUpkeep;
	
	UFUNCTION(BlueprintCallable, Category = "Turns")
	int GetCurrentTurnNumber();
	UFUNCTION(BlueprintCallable, Category = "Turns")
	void IncrementTurnNumber();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Players")
	int NumberofPlayers;

	

};
