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
	float Dpt = 0.0f;//Ducats (money per turn)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Currency")
	float Fpt = 0.0f; //food per Turn
	UFUNCTION()
	void UpdateIncome();
		
};
