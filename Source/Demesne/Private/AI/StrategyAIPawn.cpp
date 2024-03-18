// Created by WarRoom Wizards ©2023


#include "AI/StrategyAIPawn.h"

#include "EconomyComponent.h"
#include "TurnManager.h"
#include "StrategyLayerGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStrategyAIPawn::AStrategyAIPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStrategyAIPawn::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// ...
	AActor* FoundTurnManager = UGameplayStatics::GetActorOfClass(GetWorld(),ATurnManager::StaticClass());
	TurnManagerRef = Cast<ATurnManager>(FoundTurnManager);
	if(TurnManagerRef)
	{
		//Bind the EndTurn Delegate from TurnManager to the EndTurnFunction
		//This activates when the EndTurn Function in the Turn Manager is called, ideally from the End Turn Button.
		//Also requires that the Turn Manager actor be somewhere in the map and that the in Game UI is active
		//and that you have a reference to the TurnManager Actor
		TurnManagerRef->OnTurnEnd.AddUniqueDynamic(this, &ThisClass::EndTurnFunction);
	}
}

// Called every frame
void AStrategyAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStrategyAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AStrategyAIPawn::GetBehaviourTree() const
{
	return Tree;
}

void AStrategyAIPawn::EndTurnFunction()
{
	GameModeRef->EconComp->GetGold(PlayerID);
}

