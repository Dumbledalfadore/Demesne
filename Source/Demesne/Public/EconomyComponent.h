// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EconomyComponent.generated.h"


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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TArray<float> GoldBalance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	 TArray<float> FoodBalance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	 TArray<float> GoldIncome;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TArray<float> GoldUpkeep;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TArray<float> FoodIncome;//Money required per turn
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	TArray<float> FoodUpkeep;//Food required per turn
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	class ATurnManager* TurnManagerRef;
	UFUNCTION(BlueprintCallable)
	void ChangeGoldBalance(float GoldtoChange,int PlayerID);
	UFUNCTION(BlueprintCallable)
	void ChangeFoodBalance(float FoodtoChange,int PlayerID);
	UFUNCTION(BlueprintCallable)
	float GetGold(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetGold(int PlayerID,float NewBalance);
	UFUNCTION(BlueprintCallable)
	float GetFood(int PlayerID);
	UFUNCTION(BlueprintCallable)
	void SetFood(int PlayerID,float NewBalance);
	UFUNCTION()
	void EndTurnFunction();
		
};
