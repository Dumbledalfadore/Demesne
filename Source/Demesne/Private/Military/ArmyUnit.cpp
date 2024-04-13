// Created by WarRoom Wizards ©2023


#include "Military/ArmyUnit.h"

#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyDataComponent.h"
#include "Military/UnitData.h"

class AStrategyLayerGameMode;
// Sets default values
AArmyUnit::AArmyUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character Mesh"));
	//SetRootComponent(MeshComponent);

	ArmyComponent = CreateDefaultSubobject<UArmyDataComponent>(TEXT("Army Component"));

}

float AArmyUnit::CalculatePower() const
{
	float Power = 0.0f;

	int UnitCount = ArmyComponent->GetUnitDataArray().Num();
	int Index = 0;
	for(const auto Unit: ArmyComponent->GetUnitDataArray())
	{
		Power += CalculateUnitPower(Unit, Index);
		Index++;
	}

	return Power;
}

float AArmyUnit::CalculateUnitPower(const UUnitData* Unit, const int UnitIndex) const
{
	float UnitPower = 0.0f;
	if(!Unit) return UnitPower;
	
	UnitPower += Unit->GetArmour();
	UnitPower += Unit->GetMovementSpeed();
	UnitPower += Unit->GetMaxHealth();
	UnitPower += (Unit->GetMeleeAttackDamage() * Unit->GetMeleeAttackSpeed());
	UnitPower += (Unit->GetRangedAttackDamage() * Unit->GetRangedAttackSpeed());
		

	const float HealthPercentage =  ArmyComponent->GetUnitHealth(UnitIndex) / Unit->GetMaxHealth();

	/* Unit not at full health so it's not full power */
	UnitPower *= HealthPercentage;

	/* Unit's are different tiers which affects the overall power */
	UnitPower *= (1 + (static_cast<uint8>(Unit->GetUnitTier()) / 5));

	return UnitPower;
}

// Called when the game starts or when spawned
void AArmyUnit::BeginPlay()
{
	Super::BeginPlay();

	AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM && GM->TurnManager)
	{
		GM->TurnManager->OnTurnEnd.AddUniqueDynamic(this, &ThisClass::AArmyUnit::OnNextTurn);
	}

	/* Set the default */
	RemainingMovementDistance = MaxMovementDistance;

	if(ArmyComponent)
	{
		/* Add the default unit to the army */
		ArmyComponent->AddUnit(DefaultUnitData);
	}
	
}

void AArmyUnit::Destroyed()
{
	Super::Destroyed();
	
	AStrategyLayerGameMode* GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM && GM->TurnManager)
	{
		GM->TurnManager->OnTurnEnd.RemoveDynamic(this, &ThisClass::AArmyUnit::OnNextTurn);
	}
}

void AArmyUnit::OnNextTurn()
{
	/* Reset the movement distance each turn */
	RemainingMovementDistance = MaxMovementDistance;
}
