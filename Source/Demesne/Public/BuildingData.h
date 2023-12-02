#pragma once

#include "CoreMinimal.h"
#include "Resources.h"
#include "BuildingData.generated.h"

USTRUCT(BlueprintType)
struct DEMESNE_API FResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EResources Resource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ResourceAmount;
};

USTRUCT(BlueprintType)
struct DEMESNE_API FBuildingData : public FTableRowBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FResourceData UpgradeCost;

	/* Resources which the building will produce per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FResourceData> ResourcesProduced;

	/* Array of resources used to upkeep the building per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FResourceData> ResourceUpkeep;

	/* Icon to display the building as in the settlement UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
};
