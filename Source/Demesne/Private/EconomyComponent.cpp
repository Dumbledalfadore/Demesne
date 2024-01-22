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
		TurnManagerRef->OnTurnEnd.BindDynamic(this, &ThisClass::EndTurnFunction);
	}
	
}


// Called every frame
void UEconomyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UEconomyComponent::ChangeGoldBalance(int PlayerID,float GoldtoChange)
{
	//Changes the specified players gold balance by a certain amount. Use a Negative amount to remove gold and a positive amount to increase Gold
	Economy.GoldBalance.Emplace(PlayerID,GoldtoChange);
}
void UEconomyComponent::ChangeFoodBalance(int PlayerID,float FoodtoChange)
{
	//Changes the specified players Food balance by a certain amount. Use a Negative amount to remove gold and a positive amount to increase Gold
	Economy.FoodBalance.Emplace(PlayerID,FoodtoChange);
}

float UEconomyComponent::GetGold(int PlayerID)
{
	float* IDPtr = Economy.GoldBalance.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and return
		float Gold = *IDPtr;
		return Gold;
	}
	else
	{
		//if Key not found return 0.f
		return 0.f;
	}
	
}

void UEconomyComponent::SetGold(int PlayerID, float NewBalance)
{
	float* IDPtr = Economy.GoldBalance.Find(PlayerID);
	if(IDPtr)
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.GoldBalance.Emplace(PlayerID,NewBalance);
	}

}

float UEconomyComponent::GetGoldUpkeep(int PlayerID)
{
	float* IDPtr = Economy.GoldUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and return
		float Upkeep = *IDPtr;
		return Upkeep;
	}
	else
	{
		//if Key not found return 0.f
		return 0.f;
	}
}

void UEconomyComponent::SetGoldUpkeep(int PlayerID, float NewBalance)
{
	float* IDPtr = Economy.GoldUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.GoldUpkeep.Emplace(PlayerID,NewBalance);
	}
}

void UEconomyComponent::AddGoldUpkeep(int PlayerID, float Upkeep)
{
	float* IDPtr = Economy.GoldUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = *IDPtr;
		float NewUpkeep = CurrentUpkeep + Upkeep;
		Economy.GoldUpkeep.Emplace(PlayerID,NewUpkeep);
	}
}

void UEconomyComponent::SubtractGoldUpkeep(int PlayerID, float Upkeep)
{
	float* IDPtr = Economy.GoldUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = *IDPtr;
		float NewUpkeep = CurrentUpkeep - Upkeep;
		Economy.GoldUpkeep.Emplace(PlayerID,NewUpkeep);
	}
}

float UEconomyComponent::GetGoldIncome(int PlayerID)
{
	float* IDPtr = Economy.GoldIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and return
		float Income = *IDPtr;
		return Income;
	}
	else
	{
		//if Key not found return 0.f
		return 0.f;
	}
}

void UEconomyComponent::SetGoldIncome(int PlayerID, float NewBalance)
{
	float* IDPtr = Economy.GoldIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.GoldIncome.Emplace(PlayerID,NewBalance);
	}
}

void UEconomyComponent::AddGoldIncome(int PlayerID, float Income)
{
	float* IDPtr = Economy.GoldIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = *IDPtr;
		float NewIncome = CurrentIncome + Income;
		Economy.GoldIncome.Emplace(PlayerID,NewIncome);
	}
}

void UEconomyComponent::SubtractGoldIncome(int PlayerID, float Income)
{
	float* IDPtr = Economy.GoldIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = *IDPtr;
		float NewIncome = CurrentIncome - Income;
		Economy.GoldIncome.Emplace(PlayerID,NewIncome);
	}
}

float UEconomyComponent::GetFoodUpkeep(int PlayerID)
{
	float* IDPtr = Economy.FoodUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and return
		float Upkeep = *IDPtr;
		return Upkeep;
	}
	else
	{
		//if Key not found return 0.f
		return 0.f;
	}
}

void UEconomyComponent::SetFoodUpkeep(int PlayerID, float Upkeep)
{
	float* IDPtr = Economy.FoodUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.FoodUpkeep.Emplace(PlayerID,Upkeep);
	}
}

void UEconomyComponent::AddFoodUpkeep(int PlayerID, float Upkeep)
{
	float* IDPtr = Economy.FoodUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = *IDPtr;
		float NewUpkeep = CurrentUpkeep + Upkeep;
		Economy.FoodUpkeep.Emplace(PlayerID,NewUpkeep);
	}
}

void UEconomyComponent::SubtractFoodUpkeep(int PlayerID, float Upkeep)
{
	float* IDPtr = Economy.FoodUpkeep.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = *IDPtr;
		float NewUpkeep = CurrentUpkeep - Upkeep;
		Economy.FoodUpkeep.Emplace(PlayerID,NewUpkeep);
	}
}

float UEconomyComponent::GetFoodIncome(int PlayerID)
{
	float* IDPtr = Economy.FoodIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and return
		float Income = *IDPtr;
		return Income;
	}
	else
	{
		//if Key not found return 0.f
		return 0.f;
	}
}

void UEconomyComponent::SetFoodIncome(int PlayerID, float Income)
{
	float* IDPtr = Economy.FoodIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.FoodUpkeep.Emplace(PlayerID,Income);
	}
}

void UEconomyComponent::AddFoodIncome(int PlayerID, float Income)
{
	float* IDPtr = Economy.FoodIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = *IDPtr;
		float NewIncome = CurrentIncome + Income;
		Economy.FoodIncome.Emplace(PlayerID,NewIncome);
	}
}

void UEconomyComponent::SubtractFoodIncome(int PlayerID, float Income)
{
	float* IDPtr = Economy.FoodIncome.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = *IDPtr;
		float NewIncome = CurrentIncome - Income;
		Economy.FoodIncome.Emplace(PlayerID,NewIncome);
	}
}

float UEconomyComponent::GetFood(int PlayerID)
{
	float* IDPtr = Economy.FoodBalance.Find(PlayerID);
	if(IDPtr)
	{
		//Get Value at Key and return
		float Food = *IDPtr;
		return Food;
	}
	else
	{
		//if Key not found return 0.f
		return 0.f;
	}
}

void UEconomyComponent::SetFood(int PlayerID,float NewBalance)
{
	float* IDPtr = Economy.FoodBalance.Find(PlayerID);
	if(IDPtr)
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.FoodBalance.Emplace(PlayerID,NewBalance);
	}
}

void UEconomyComponent::EndTurnFunction()
{
 //This is where anything to do with the economy at the end of a turn happens
	if(GameModeRef)
	{
		for(int i = 0; i <= GameModeRef->NumberofPlayers -1; i++)
		{
			ChangeGoldBalance(i,EconHelper::CalculateRevenue(GetGoldIncome(i),GetGoldUpkeep(i)));
			ChangeFoodBalance(i,EconHelper::CalculateRevenue(GetFoodIncome(i),GetFoodUpkeep(i)));
			
		}
	}
	
}

