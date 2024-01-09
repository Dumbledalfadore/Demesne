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

/* 'Resources' used locally within settlements */
UENUM(BlueprintType)
enum class EBonusSources : uint8
{
	BuildingCap, /* Should only work when applied to Settlement building types */
	Growth
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
 * NOTE: Any new building tiers require GetNextBuildingTier() to be modified within the Settlement class
 *       to prevent errors
 */
UENUM(BlueprintType)
enum class EBuildingTier : uint8
{
	Tier0 = 0 UMETA(DisplayName = "I"),
	Tier1 = 1 UMETA(DisplayName = "II"),
	Tier2 = 2 UMETA(DisplayName = "III"),
	Tier3 = 3 UMETA(DisplayName = "IV"),
	Tier4 = 4 UMETA(DisplayName = "V"),
};

/* Types of buildings */
UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Settlement = 0 UMETA(DisplayName = "Settlement"),
	Farming = 1 UMETA(DisplayName = "Farming"),
	Military = 2 UMETA(DisplayName = "Military"),
};

/* STRUCTS */ //////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Used to add bonuses or restrictions to buildings
 * EXAMPLE: A settlement might have an overall cap of 8
 *			however, you could add 'BuildingCap' with a value of 6 to a settlement building
 *			to cap the amount of current slots, so the extra 2 slots would be locked
 */
USTRUCT(BlueprintType)
struct DEMESNE_API FBonusData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBonusSources Bonus;

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

