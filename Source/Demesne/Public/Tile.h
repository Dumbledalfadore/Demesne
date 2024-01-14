// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UStaticMeshComponent;

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

protected:
	// Called when the game starts or when spawned
	
	UPROPERTY(BlueprintReadWrite, Category = "Tile")
	eTileType tileType;

	UPROPERTY(EditAnywhere, Category = "Tile")
	UStaticMeshComponent* tileMesh;



public:	


	UFUNCTION()
	void SpawnBuilding();
};
