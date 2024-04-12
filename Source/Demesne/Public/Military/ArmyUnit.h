// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmyUnit.generated.h"

class ATile;
class UUnitData;
class UArmyDataComponent;

UCLASS()
class DEMESNE_API AArmyUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmyUnit();
	
	UFUNCTION()
	int GetPlayerID() const { return PlayerID; }

	UFUNCTION()
	void SetPlayerID(const int ID) { PlayerID = ID; }

	UFUNCTION()
	int32 GetMaxMovementDistance() const { return MaxMovementDistance; }

	UFUNCTION()
	UArmyDataComponent* GetArmyComponent() const { return ArmyComponent; }

	UFUNCTION()
	UUnitData* GetDefaultUnitData() { return DefaultUnitData; }

	UFUNCTION()
	TArray<UUnitData*> GetRecruitableUnits() { return RecruitableUnits; }

	UFUNCTION()
	FIntPoint GetTileIndex() const { return CurrentTileIndex; }

	UFUNCTION()
	void SetTileIndex(const FIntPoint Index) { CurrentTileIndex = Index; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	/* ID of the controlling player */
	UPROPERTY()
	int PlayerID = -1;

	UPROPERTY()
	FIntPoint CurrentTileIndex;

	UPROPERTY(EditAnywhere)
	int32 MaxMovementDistance;

	/* Holds data about the troops in the garrison */
	UPROPERTY(VisibleAnywhere)
	class UArmyDataComponent* ArmyComponent;

	/* The unit that 'leads' the army */
	UPROPERTY(EditAnywhere)
	class UUnitData* DefaultUnitData;

	/* The units that can be recruited for the army */
	UPROPERTY(EditAnywhere)
	TArray<class UUnitData*> RecruitableUnits;
};
