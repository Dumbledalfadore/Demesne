// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SettlementManager.generated.h"

UCLASS()
class DEMESNE_API ASettlementManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASettlementManager();

	/* Can be used if we want a menu showing the player all of their owned settlements */
	UFUNCTION()
	TArray<class ASettlement*> GetSettlementsByID(int32 PlayerID);
	
	UFUNCTION()
	TArray<class ASettlement*> GetAllSettlements();

	/* Should be used when a settlement is captured */
	UFUNCTION()
	void HandoverSettlement(ASettlement* Settlement, int32 ToPlayerID);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<class ASettlement*> Settlements;
};
