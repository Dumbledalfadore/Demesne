// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_HexTile.generated.h"

class UStaticMeshComonent;

UENUM()
enum class eHexTileType : uint8
{
	INVALID,
	Grass,
	Water,
};

UCLASS()
class DEMESNE_API AC_HexTile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Tile")
	eHexTileType tileType;

	UPROPERTY(EditAnywhere, Category = "Tile")
	UStaticMeshComponent* tileMesh;
public:	
	// Sets default values for this actor's properties
	AC_HexTile();

	UPROPERTY(VisibleAnywhere)
	FIntPoint tileIndex;
};
