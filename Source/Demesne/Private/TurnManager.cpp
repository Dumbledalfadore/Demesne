// Created by WarRoom Wizards ©2023


#include "TurnManager.h"
#include "StrategyLayerGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurnManager::ATurnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//OnTurnEnd.AddUniqueDynamic(this,&ATurnManager::OnTurnEndFunction);
}

// Called when the game starts or when spawned
void ATurnManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("Turn Manager Loaded"));
}

void ATurnManager::OnTurnEndEvent()
{
	
}

// Called every frame
void ATurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurnManager::OnTurnEndFunction()
{
	UE_LOG(LogTemp,Warning,TEXT("Broadcast Called"));
	AStrategyLayerGameMode* GameMode = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(this));
	if(GameMode){GameMode->IncrementTurnNumber();}
	
	OnTurnEnd.Broadcast();
}

