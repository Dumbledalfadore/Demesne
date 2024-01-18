// Created by WarRoom Wizards ©2023


#include "TurnManager.h"
#include "StrategyLayerGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurnManager::ATurnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ATurnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATurnManager::OnTurnEndEvent()
{
	OnTurnEnd.ExecuteIfBound();
}

// Called every frame
void ATurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurnManager::OnTurnEndFunction()
{
	AStrategyLayerGameMode* GameMode = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode){GameMode->IncrementTurnNumber();}
	OnTurnEndEvent();
}

