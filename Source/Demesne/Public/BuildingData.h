#pragma once

#include "CoreMinimal.h"
#include "DemenseStructAndEnums.h"
#include "BuildingData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DEMESNE_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString BuildingName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBuildingTier BuildingTier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBuildingType BuildingType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FResourceData UpgradeCost;

	/* Resources which the building will produce per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FResourceData> ResourcesProduced;

	/* Array of resources used to upkeep the building per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FResourceData> ResourceUpkeep;

	/* Array of any bonuses or limitations given to the settlement from having the building */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBonusData> BuildingModifiers;

	/* Icon to display the building as in the settlement UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
};
