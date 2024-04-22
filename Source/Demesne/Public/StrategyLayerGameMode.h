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

	int mTurn = 1;
	int mMaxTurns = 200;

public:
	UPROPERTY()
	class ADemesnePlayerController* Controller;
	UFUNCTION(BlueprintCallable, Category = "Economy")
	int GetCurrentTurn();
	UFUNCTION(BlueprintCallable, Category = "Economy")
	int GetMaximumTurn();
	
	UFUNCTION()
	void CheckConditions();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UEconomyComponent* EconComp;

	UPROPERTY(BlueprintReadOnly)
	class ASettlementManager* SettlementManager;

	UPROPERTY(BlueprintReadOnly)
	class AArmyManager* ArmyManager;

	UPROPERTY(BlueprintReadOnly)
	class AGridManager* GridManager;

	UPROPERTY(BlueprintReadOnly)
	class ATurnManager* TurnManager;

	UPROPERTY(BlueprintReadOnly)
	class APathfinder* PathFinder;

	UPROPERTY(BlueprintReadOnly)
	class AStrategyAIPawn* AIPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASettlementManager> SettlementManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AArmyManager> ArmyManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGridManager> GridManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATurnManager> TurnManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APathfinder> PathFinderClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AStrategyAIPawn> StrategyAIClass;
	
	//Amount of Gold and Food that Players and AI start with
	
	UFUNCTION(BlueprintCallable, Category = "Turns")
	int GetCurrentTurnNumber();
	UFUNCTION(BlueprintCallable, Category = "Turns")
	void IncrementTurnNumber();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Players")
	int NumberofPlayers;

	bool GameWon = false;
	bool GameLost = false;

};
