// Fill out your copyright notice in the Description page of Project Settings.
#include "EconomyComponent.h"

#include "EconHelper.h"
#include "TurnManager.h"
#include "StrategyLayerGameMode.h"
#include "Kismet/GameplayStatics.h"



// Sets default values for this component's properties
UEconomyComponent::UEconomyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEconomyComponent::BeginPlay()
{
	Super::BeginPlay();
	 GameModeRef = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// ...
	if(TurnManagerRef)
	{
		//Bind the EndTurn Delegate from TurnManager to the EndTurnFunction
		//This activates when the EndTurn Function in the Turn Manager is called, ideally from the End Turn Button.
		//Also requires that the Turn Manager actor be somewhere in the map and that the in Game UI is active
		TurnManagerRef->OnTurnEnd.BindUObject(this,&UEconomyComponent::EndTurnFunction);
	}
	
}


// Called every frame
void UEconomyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UEconomyComponent::ChangeGoldBalance(float GoldtoChange, int PlayerID)
{
	//Changes the specified players gold balance by a certain amount. Use a Negative amount to remove gold and a positive amount to increase Gold
	GoldBalance[PlayerID] += GoldtoChange;
}
void UEconomyComponent::ChangeFoodBalance(float FoodtoChange, int PlayerID)
{
	//Changes the specified players Food balance by a certain amount. Use a Negative amount to remove gold and a positive amount to increase Gold
	FoodBalance[PlayerID] += FoodtoChange;
}

float UEconomyComponent::GetGold(int PlayerID)
{
	if (PlayerID <= GoldBalance.Num())
	{
		return GoldBalance[PlayerID];
	}
	else
	{
		return 0.f;
	}
	
	
}

void UEconomyComponent::SetGold(int PlayerID, float NewBalance)
{
	if (PlayerID <= GoldBalance.Num())
	{
		GoldBalance[PlayerID] = NewBalance;
	}

}

float UEconomyComponent::GetFood(int PlayerID)
{
	if (PlayerID <= FoodBalance.Num())
	{
		return FoodBalance[PlayerID];
	}
	else
	{
		return 0.f;
	}
}

void UEconomyComponent::SetFood(int PlayerID,float NewBalance)
{
	if (PlayerID <= FoodBalance.Num())
	{
		FoodBalance[PlayerID] = NewBalance;
	}
}

void UEconomyComponent::EndTurnFunction()
{
 //This is where anything to do with the economy at the end of a turn happens
	if(GameModeRef)
	{
		for(int i = 0; i <= GameModeRef->NumberofPlayers -1; i++)
		{
			ChangeGoldBalance(EconHelper::CalculateRevenue(GoldIncome[i],GoldUpkeep[i]),i);
			ChangeGoldBalance(EconHelper::CalculateRevenue(FoodIncome[i],FoodUpkeep[i]),i);
		}
	}
	
}

