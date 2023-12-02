// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Settlement.generated.h"



UENUM(BlueprintType)
enum class ESettlementType : uint8
{
	Camp,
	Minor,
	Major
};

UCLASS()
class DEMESNE_API ASettlement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASettlement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	//UBoxComponent* Collider;

	/* Static mesh of the settlement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	/* The population of the settlement determines which buildings can be built and how many building slots can be used.
	   Subtracted from when a building is demolished so the growth needed is slightly less */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	int SettlementPopulation;

	/* The surplus population of the settlement - spent on buildings and not refunded when a building is demolished */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	int SettlementSurplus;

	/* The current rate at which the growth is increased per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float CurrentGrowthRate;

	/* The current amount of growth accumulated */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float AccumulatedGrowth;

	/* The base amount of growth */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Population")
	float BaseGrowthRequired;

	/* Determines how many building slots a settlement will have and what the maximum build tier is */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement")
	ESettlementType SettlementType;

	/* How many tiers and how many building slots this settlement will have - set by the settlement type.
	* Camp - 2 Slots
	* Minor - 3 Slots
	* Major - 5 Slots
	* Data can still be assigned to the different tier arrays however will only be read if it matches this value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	uint8 BuildingCap;

	/* Used to track how many buildings we currently have, rather than calling CurrentBuildings.Num() is .IsEmpty() */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	uint8 BuildingCount;

	/* Array containing the different possible buildings and tiers that can be built for the settlement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	TArray<UDataTable*> AvailableBuildings;

	/* TMap containing the current buildings of the settlement
	   TMap<AvailableBuildingIndex, BuildingTier> */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settlement|Buildings")
	TMap<int, int> CurrentBuildings;

	/* Called on each turn, adds all turn based resources from buildings etc */
	UFUNCTION()
	void NextTurn();

	/* Finds the buildings which are available to be built */
	UFUNCTION()
	void FindAvailableBuildings();

	UFUNCTION()
	void BuildBuilding(int BuildingIndex);

	//UFUNCTION()
	//void UpgradeBuilding();

};
