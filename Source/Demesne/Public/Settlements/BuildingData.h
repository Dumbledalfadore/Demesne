#pragma once

#include "CoreMinimal.h"
#include "DemenseStructAndEnums.h"
#include "BuildingData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DEMESNE_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:
	/* A string used to identify buildings of the same chain so different tiers of the same building cant be built
	 * EXAMPLE Upgrade a farm building from tier 1 to 2 would allow the tier 1 to enter the buildings to build pool again
	 * however we can do a check against this string to see if they belong to the same chain and prevent the build
	 */
	UPROPERTY(EditAnywhere)
	FString BuildingIdentifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString BuildingName;

	/* #OPTIONAL: A description to show on the tooltip, resources are automatically generated for the tooltip */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString BuildingDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBuildingTier BuildingTier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBuildingType BuildingType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FResourceData UpgradeCost;

	/* Resources it will cost to build the building */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FResourceData> ResourcesToBuild;
	
	/* Resources which the building will produce or consume per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FResourceData> ResourcesPerTurn;
	
	/* Array of any bonuses or limitations given to the settlement from having the building */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLocalResourceData> BuildingModifiers;

	/* Array holding potential upgrades for this building */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UBuildingData*> BuildingUpgrades;

	/* Icon to display the building as in the settlement UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
};
