// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "DemenseStructAndEnums.generated.h"


/* ENUMS */ ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Resources used for the wider gameplay */
UENUM(BlueprintType)
enum class EResourceType : uint8 {
	Gold = 0 UMETA(DisplayName = "Gold"),
	Food = 1  UMETA(DisplayName = "Food")
};

/* Resources used locally within settlements */
UENUM(BlueprintType)
enum class ELocalResourceType : uint8
{
	BuildingCap = 0 UMETA(DisplayName = "Building Cap"), /* Should only work when applied to Settlement building types */
	Growth = 1 UMETA(DisplayName = "Growth"),
	Food = 2 UMETA(DisplayName = "Food"), /* Used to hold how much food is generated locally within a settlement */
	Gold = 3 UMETA(DisplayName = "Gold"), /* USed to hold how much gold is generated locally within a settlement */
	Population = 4 UMETA(DisplayName = "Population Surplus"),
};

/* Types of settlements */
UENUM(BlueprintType)
enum class ESettlementType : uint8
{
	Camp,
	Minor,
	Major
};

/* Tiers of buildings
 * NOTE: If you create any new building tiers,  then GetNextBuildingTier() needs to be modified within the Settlement class
 *       to prevent errors
 */
UENUM(BlueprintType)
enum class EBuildingTier : uint8
{
	Expand = 0 UMETA(DisplayName = "Expand"),
	Build = 1 UMETA(DisplayName = "Build"),
	Tier1 = 2 UMETA(DisplayName = "I"),
	Tier2 = 3 UMETA(DisplayName = "II"),
	Tier3 = 4 UMETA(DisplayName = "III"),
	Tier4 = 5 UMETA(DisplayName = "IV"),
	Tier5 = 6 UMETA(DisplayName = "V"),
	Destroy = 7 UMETA(DisplayName = "Deconstruct"),
};

/* Types of buildings
 * NOTE: New building types will need an array within the settlement to hold the UDataAssets of the buildings
 * NOTE: GetBuildingsToBuild() will also need to be modified to add the new buildings array
 * NOTE: GetBuildingsByType() will need a new case added to return the building array
 */
UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Misc = 0 UMETA(DisplayName = "Misc"),
	Settlement = 1 UMETA(DisplayName = "Settlement"),
	Farming = 2 UMETA(DisplayName = "Farming"),
	Military = 3 UMETA(DisplayName = "Military"),
	Fishing = 4 UMETA(DisplayName = "Fishing"),
	Craft = 5 UMETA(DisplayName = "Craft"),
};

/* STRUCTS */ //////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Used to add bonuses or restrictions to buildings
 * EXAMPLE: A settlement might have an overall cap of 8
 *			however, you could add 'BuildingCap' with a value of 6 to a settlement building
 *			to cap the amount of current slots, so the extra 2 slots would be locked
 */
USTRUCT(BlueprintType)
struct DEMESNE_API FLocalResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ELocalResourceType Bonus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BonusValue;
};

/* Used to set resource type and amount for example,
 * the type of resource and amount used to upkeep a certain building
 * within settlements
 */
USTRUCT(BlueprintType)
struct DEMESNE_API FResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EResourceType Resource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ResourceAmount;
};

