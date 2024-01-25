// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnManager.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnEndDelegate);
UCLASS()
class DEMESNE_API ATurnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void OnTurnEndEvent();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
	FOnTurnEndDelegate OnTurnEnd;
	UFUNCTION(BlueprintCallable)
	void OnTurnEndFunction();
	

};
