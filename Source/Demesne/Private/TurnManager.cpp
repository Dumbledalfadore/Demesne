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
	//Set player turn now so the player always goes first
	TurnState = EPLAYERTURN;
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
	//if its player turn proceed as normal, else do not proceed.
	//This is so the player can't spam end turn and allows the AI a chance to complete its turn.
	//TurnState is planned to revert back to player turn after a 3 second delay after the EndTurn button is pressed in the WBP_EndTurn graph
	if(TurnState == EPLAYERTURN)
	{
		if(GameMode){GameMode->IncrementTurnNumber();}
		TurnState = ECOMPUTERTURN;
		OnTurnEnd.Broadcast();
	}
	
}

