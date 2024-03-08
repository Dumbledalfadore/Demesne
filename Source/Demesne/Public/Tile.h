// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UStaticMeshComponent;
class APlaceHolder;
class ASettlement;
UENUM()
enum class eTileType : uint8
{
	INVALID,
	Grass,
	Water,
};

UCLASS()
class DEMESNE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(VisibleAnywhere)
	FIntPoint tileIndex;
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	eTileType tileType;
protected:
	// Called when the game starts or when spawned
	
	

	UPROPERTY(EditAnywhere, Category = "Tile")
	UStaticMeshComponent* tileMesh;



public:	
	

	UFUNCTION()
	void SpawnBuilding(int teamNum, TSubclassOf<ASettlement> buildingType, bool isMajor);

	UFUNCTION()
	void SpawnFlora();

	UPROPERTY(VisibleAnywhere)
	bool hasPlant = false;

	UPROPERTY(VisibleAnywhere)
	bool hasBuilding = false;

	UPROPERTY(VisibleAnywhere)
	bool isLand = false;

	UPROPERTY(VisibleAnywhere)
	int ownerTeamNum;


private:

	UPROPERTY()
	FVector placeHolderPos = FVector(0.0f, 0.0f, 90.0f);

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlaceHolder> floraPlaceholder;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APlaceHolder> buildingPlaceholder;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* OnTileSpawnLocation;
};
