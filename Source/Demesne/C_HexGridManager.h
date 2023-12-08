// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_HexGridManager.generated.h"

class AC_HexTile;

UCLASS()
class DEMESNE_API AC_HexGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_HexGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	int32 GridHeight;
	UPROPERTY(EditAnywhere)
	int32 GridWidth;

	UPROPERTY(EditAnywhere)
	float TileHorOffset = 173.205078f;

	UPROPERTY(EditAnywhere)
	float TileVerOffset = 150.0f;

	UPROPERTY(EditAnywhere)
	float OddHorOffset = 86.602539;

	UPROPERTY(EditAnywhere)
	float OddVerOffset;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AC_HexTile> GrassTile;


	TArray<TArray<AC_HexTile*>> GridArray;
public:	
	
private:
	UFUNCTION()
	void InitGrid();

};
