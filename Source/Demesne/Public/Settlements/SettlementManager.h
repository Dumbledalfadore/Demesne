// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SettlementManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSettlementUpdated);

UCLASS()
class DEMESNE_API ASettlementManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASettlementManager();

	UPROPERTY()
	FSettlementUpdated OnSettlementUpdated;

	/* Can be used if we want a menu showing the player all of their owned settlements */
	UFUNCTION()
	TArray<class ASettlement*> GetSettlementsByID(const int PlayerID);

	UFUNCTION()
	TArray<class ASettlement*> GetSettlements();
	
	UFUNCTION()
	void AddSettlement(ASettlement* Settlement);

	/* Should be used when a settlement is captured */
	UFUNCTION()
	void HandoverSettlement(ASettlement* Settlement, const int FromPlayerID, const int ToPlayerID);

	UFUNCTION()
	FString GetSettlementName();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class AStrategyLayerGameMode* GM;

	UPROPERTY()
	TArray<class ASettlement*> Settlements;

	UPROPERTY()
	TArray<FString> SettlementNames; 
};
