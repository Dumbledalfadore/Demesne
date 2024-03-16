// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitData.generated.h"

struct FResourceData;
enum class EUnitTier : uint8;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DEMESNE_API UUnitData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	FName GetUnitName() const { return UnitName; }

	UFUNCTION()
	FString GetUnitDescription() { return UnitDescription; }

	UFUNCTION()
	UTexture2D* GetIcon() const { return UnitIcon; }

	UFUNCTION()
	EUnitTier GetUnitTier() const { return UnitTier; }

	UFUNCTION()
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION()
	float GetArmour() const { return Armour; }

	UFUNCTION()
	float GetMeleeAttackDamage() const { return MeleeAttackDamage; }
	
	UFUNCTION()
	float GetMeleeAttackSpeed() const { return MeleeAttackSpeed; }

	UFUNCTION()
	float GetMovementSpeed() const { return MovementSpeed; }

	UFUNCTION()
	float GetRangedAttackDamage() const { return RangedAttackDamage; }

	UFUNCTION()
	float GetRangedAttackSpeed() const { return RangedAttackSpeed; }

	UFUNCTION()
	float GetFoodUpkeep() const { return FoodUpkeep; }

	UFUNCTION()
	float GetGoldUpkeep() const { return GoldUpkeep; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName UnitName;

	/* #OPTIONAL: A description to show on the tooltip */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString UnitDescription;

	/* Icon to display the unit as in the UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* UnitIcon;

	/* Tier of the unit, will affect stats */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUnitTier UnitTier;

	/* Stats for the unit ------------------- */
	/* Maximum health of the unit, Current Health is stored in the ArmyDataComponent */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Armour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeleeAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeleeAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RangedAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RangedAttackSpeed;
	
	/* Cost */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FoodUpkeep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GoldUpkeep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GoldCostToCreate;

	/* Multiplier applied to GoldCostToCreate when the unit will be the first spawned (General/Leader) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GoldCostMultiplier;

	
};
