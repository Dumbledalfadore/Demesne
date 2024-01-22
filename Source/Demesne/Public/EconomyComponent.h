// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EconomyComponent.generated.h"

USTRUCT(BlueprintType)
struct FEconomyStruct
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TMap<int32,float>GoldBalance;//Gold current amount
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TMap<int32,float>FoodBalance;//Food current amount
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TMap<int32,float> GoldIncome;//Gold received per turn
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TMap<int32,float> GoldUpkeep;//Money required per turn
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TMap<int32,float> FoodIncome;//Food received per turn
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TMap<int32,float> FoodUpkeep;//Food required per turn
	
	
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMESNE_API UEconomyComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEconomyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	class AStrategyLayerGameMode* GameModeRef;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	class ATurnManager* TurnManagerRef;
	UFUNCTION(BlueprintCallable)
	void ChangeGoldBalance(int PlayerID,float GoldtoChange);
	UFUNCTION(BlueprintCallable)
	void ChangeFoodBalance(int PlayerID,float FoodtoChange);
	UFUNCTION(BlueprintCallable)
	float GetGold(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetGold(int PlayerID,float NewBalance);
	UFUNCTION(BlueprintCallable)
	float GetGoldUpkeep(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetGoldUpkeep(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	void AddGoldUpkeep(int PlayerID,float Upkeep);
	UFUNCTION(BlueprintCallable)
	void SubtractGoldUpkeep(int PlayerID,float Upkeep);
	UFUNCTION(BlueprintCallable)
	float GetGoldIncome(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetGoldIncome(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	void AddGoldIncome(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	void SubtractGoldIncome(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	float GetFoodUpkeep(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetFoodUpkeep(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	void AddFoodUpkeep(int PlayerID,float Upkeep);
	UFUNCTION(BlueprintCallable)
	void SubtractFoodUpkeep(int PlayerID,float Upkeep);
	UFUNCTION(BlueprintCallable)
	float GetFoodIncome(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetFoodIncome(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	void AddFoodIncome(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	void SubtractFoodIncome(int PlayerID,float Income);
	UFUNCTION(BlueprintCallable)
	float GetFood(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetFood(int PlayerID,float NewBalance);
	UFUNCTION()
	void EndTurnFunction();
private:
	FEconomyStruct Economy;
		
};
