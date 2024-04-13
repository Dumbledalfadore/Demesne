// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "ArmyUnit.h"
#include "GameFramework/Actor.h"
#include "ArmyManager.generated.h"

class AStrategyLayerGameMode;
class AArmyUnit;

UCLASS()
class DEMESNE_API AArmyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmyManager();

	UFUNCTION()
	void CreateArmy(const FIntPoint TileIndex, const TSubclassOf<AArmyUnit> UnitToSpawn, const int PlayerID);

	UFUNCTION()
	void InitiateBattle(AArmyUnit* Aggressor, AArmyUnit* Defender);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* List of all current armies */
	UPROPERTY(VisibleAnywhere)
	TMap<int, AArmyUnit*> ActiveArmies;
	
	UPROPERTY()
	AStrategyLayerGameMode* GM;
};
