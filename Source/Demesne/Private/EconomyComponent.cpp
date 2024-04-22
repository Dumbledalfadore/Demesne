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
	
	// ...
	
	
	
}


// Called every frame
void UEconomyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UEconomyComponent::InitEconomyMaps(int PlayerID)
{
	//Initialise all Economy Maps for the specified Player ID using the StarterVariables
	//UE_LOG(LogTemp,Warning,TEXT("EconComp Init Called"));
	if(PlayerID != 0.f)
	{
		//If the player is an AI(human always has ID of 0, multiply by the AI Modifier. This allows the AI to have a different starting amount while being easily changeable by any future difficulty mechanics
		Economy.GoldBalance.Add(PlayerID,BaseGold * AIModifier);
		Economy.GoldIncome.Add(PlayerID,BaseGoldIncome* AIModifier);
		Economy.GoldUpkeep.Add(PlayerID,BaseGoldUpkeep* AIModifier);
		Economy.FoodBalance.Add(PlayerID,BaseFood* AIModifier);
		Economy.FoodIncome.Add(PlayerID,BaseFoodIncome* AIModifier);
		Economy.FoodUpkeep.Add(PlayerID,BaseFoodUpkeep* AIModifier);
	}
	else
	{
		//If player add as normal
		Economy.GoldBalance.Add(PlayerID,BaseGold);
		Economy.GoldIncome.Add(PlayerID,BaseGoldIncome);
		Economy.GoldUpkeep.Add(PlayerID,BaseGoldUpkeep);
		Economy.FoodBalance.Add(PlayerID,BaseFood);
		Economy.FoodIncome.Add(PlayerID,BaseFoodIncome);
		Economy.FoodUpkeep.Add(PlayerID,BaseFoodUpkeep);
	}
	
	
	
}

void UEconomyComponent::InitTurnManger()
{
	GameModeRef = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
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
	for(int i = 0; i <= GameModeRef->NumberofPlayers -1; i++)
	{
		
		InitEconomyMaps(i);
	}
}

//DEPRECATED:USE SETGOLD
void UEconomyComponent::ChangeGoldBalance(int PlayerID,float GoldtoChange)
{
	
	Economy.GoldBalance.Add(PlayerID,GoldtoChange);
}
//DEPRECATED: USE SETFOOD
void UEconomyComponent::ChangeFoodBalance(int PlayerID,float FoodtoChange)
{
	
	Economy.FoodBalance.Add(PlayerID,FoodtoChange);
}

float UEconomyComponent::GetGold(int PlayerID)
{
	//Get Gold
	if(Economy.GoldBalance.Contains(PlayerID))
	{
		//Get Value at Key and return
		float Gold = Economy.GoldBalance.FindRef(PlayerID);
		//UE_LOG(LogTemp,Warning,TEXT("Gold: %f"),Gold);
		return Gold;
	}
	else
	{
		//if Key not found return 0.f
		return 18107.f;
	}
	
}

void UEconomyComponent::SetGold(int PlayerID, float NewBalance)
{
	//Set Gold to a certain amount, mainly for debugging
	if(Economy.GoldBalance.Contains(PlayerID))
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.GoldBalance.Add(PlayerID,NewBalance);
	}

}

void UEconomyComponent::AddGold(int PlayerID, float GoldToAdd)
{
	//get a reference to the current gold amount then adds the parameter value before setting it as the gold value
	if(Economy.GoldBalance.Contains(PlayerID))
	{
		float NewGoldSum = Economy.GoldBalance.FindRef(PlayerID) + GoldToAdd;
		Economy.GoldBalance.Add(PlayerID,NewGoldSum);
	}
}

void UEconomyComponent::SubtractGold(int PlayerID, float GoldToSubtract)
{
	//same as above but subtraction instead of addition
	if(Economy.GoldBalance.Contains(PlayerID))
	{
		float NewGoldSum = Economy.GoldBalance.FindRef(PlayerID) - GoldToSubtract;
		Economy.GoldBalance.Add(PlayerID,NewGoldSum);
	}
}

float UEconomyComponent::GetGoldUpkeep(int PlayerID)
{
	//GetUpkeep
	if(Economy.GoldUpkeep.Contains(PlayerID))
	{
		//Get Value at Key and return
		float Upkeep = Economy.GoldUpkeep.FindRef(PlayerID);
		return Upkeep;
	}
	else
	{
		//if Key not found return 0.f
		return 0.7f;
	}
}

void UEconomyComponent::SetGoldUpkeep(int PlayerID, float NewBalance)
{
	//Set Upkeep
	if(Economy.GoldUpkeep.Contains(PlayerID))
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.GoldUpkeep.Add(PlayerID,NewBalance);
	}
}

void UEconomyComponent::AddGoldUpkeep(int PlayerID, float Upkeep)
{
	//add upkeep by Param amount
	if(Economy.GoldUpkeep.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = Economy.GoldUpkeep.FindRef(PlayerID);
		float NewUpkeep = CurrentUpkeep + Upkeep;
		Economy.GoldUpkeep.Add(PlayerID,NewUpkeep);
	}
}

void UEconomyComponent::SubtractGoldUpkeep(int PlayerID, float Upkeep)
{
	// Lower Upkeep by param amount
	if(Economy.GoldUpkeep.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = Economy.GoldUpkeep.FindRef(PlayerID);
		float NewUpkeep = CurrentUpkeep - Upkeep;
		Economy.GoldUpkeep.Add(PlayerID,NewUpkeep);
	}
}

float UEconomyComponent::GetGoldIncome(int PlayerID)
{
	//Get Income
	if(Economy.GoldIncome.Contains(PlayerID))
	{
		//Get Value at Key and return
		float Income = Economy.GoldIncome.FindRef(PlayerID);
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
	//Set Income
	if(Economy.GoldIncome.Contains(PlayerID))
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.GoldIncome.Add(PlayerID,NewBalance);
	}
}

void UEconomyComponent::AddGoldIncome(int PlayerID, float Income)
{
	//Increase Income
	if(Economy.GoldIncome.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = Economy.GoldIncome[PlayerID];
		float NewIncome = CurrentIncome + Income;
		Economy.GoldIncome.Add(PlayerID,NewIncome);
	}
}

void UEconomyComponent::SubtractGoldIncome(int PlayerID, float Income)
{
	//Decrease Income
	if(Economy.GoldIncome.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = Economy.GoldIncome[PlayerID];;
		float NewIncome = CurrentIncome - Income;
		Economy.GoldIncome.Add(PlayerID,NewIncome);
	}
}

float UEconomyComponent::GetFoodUpkeep(int PlayerID)
{
	//Get the current food upkeep
	if(Economy.FoodUpkeep.Contains(PlayerID))
	{
		//Get Value at Key and return
		float Upkeep = Economy.FoodUpkeep.FindRef(PlayerID);
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
	
	if(Economy.FoodUpkeep.Contains(PlayerID))
	{
		//Similar Function as ChangeBalance but more Type Safe and for upkeep
		Economy.FoodUpkeep.Add(PlayerID,Upkeep);
	}
}

void UEconomyComponent::AddFoodUpkeep(int PlayerID, float Upkeep)
{
	//Add to the current upkeep
	if(Economy.FoodUpkeep.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = Economy.FoodUpkeep.FindRef(PlayerID);
		float NewUpkeep = CurrentUpkeep + Upkeep;
		Economy.FoodUpkeep.Add(PlayerID,NewUpkeep);
	}
}

void UEconomyComponent::SubtractFoodUpkeep(int PlayerID, float Upkeep)
{
	
	if(Economy.FoodUpkeep.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentUpkeep = Economy.FoodUpkeep.FindRef(PlayerID);
		float NewUpkeep = CurrentUpkeep - Upkeep;
		Economy.FoodUpkeep.Add(PlayerID,NewUpkeep);
	}
}

float UEconomyComponent::GetFoodIncome(int PlayerID)
{
	
	if(Economy.FoodIncome.Contains(PlayerID))
	{
		//Get Value at Key and return
		float Income = Economy.FoodIncome.FindRef(PlayerID);
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
	
	if(Economy.FoodIncome.Contains(PlayerID))
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.FoodIncome.Add(PlayerID,Income);
	}
}

void UEconomyComponent::AddFoodIncome(int PlayerID, float Income)
{
	
	if(Economy.FoodIncome.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = Economy.FoodIncome.FindRef(PlayerID);
		float NewIncome = CurrentIncome + Income;
		Economy.FoodIncome.Add(PlayerID,NewIncome);
	}
}

void UEconomyComponent::SubtractFoodIncome(int PlayerID, float Income)
{

	if(Economy.FoodIncome.Contains(PlayerID))
	{
		//Get Value at Key and add new upkeep
		float CurrentIncome = Economy.FoodIncome.FindRef(PlayerID);
		float NewIncome = CurrentIncome - Income;
		Economy.FoodIncome.Add(PlayerID,NewIncome);
	}
}

float UEconomyComponent::GetFood(int PlayerID)
{
	
	if(Economy.FoodBalance.Contains(PlayerID))
	{
		//Get Value at Key and return
		float Food = Economy.FoodBalance.FindRef(PlayerID);
		return Food;
	}
	else
	{
		//if Key not found return num thats obviously an error
		return 12000.f;
	}
}

void UEconomyComponent::SetFood(int PlayerID,float NewBalance)
{
	
	if(Economy.FoodBalance.Contains(PlayerID))
	{
		//Similar Function as ChangeBalance but more Type Safe
		Economy.FoodBalance.Add(PlayerID,NewBalance);
	}
}

void UEconomyComponent::AddFood(int PlayerID, float FoodToAdd)
{
	if(Economy.FoodBalance.Contains(PlayerID))
	{
		float NewFoodSum = Economy.FoodBalance.FindRef(PlayerID) + FoodToAdd;
		Economy.FoodBalance.Add(PlayerID,NewFoodSum);
	}
}

void UEconomyComponent::SubtractFood(int PlayerID, float FoodToSubtract)
{
	if(Economy.FoodBalance.Contains(PlayerID))
	{
		float NewFoodSum = Economy.FoodBalance.FindRef(PlayerID) - FoodToSubtract;
		Economy.FoodBalance.Add(PlayerID,NewFoodSum);
	}
}

float UEconomyComponent::GetPlayerGoldRevenue(int PlayerID)
{
	//Subtract upkeep from income to get the total amount the balance should change by this turn
	if(Economy.GoldUpkeep.Contains(PlayerID) && Economy.GoldIncome.Contains(PlayerID))
	{
		float currentGold = Economy.GoldBalance.FindRef(PlayerID);
		float income = Economy.GoldIncome.FindRef(PlayerID);
		float outcome = Economy.GoldUpkeep.FindRef(PlayerID);
		float revenue = income - outcome;
		return revenue;
	}
	else {return 0.0f;}
}

float UEconomyComponent::GetPlayerFoodRevenue(int PlayerID)
{
	if(Economy.FoodUpkeep.Contains(PlayerID) && Economy.FoodIncome.Contains(PlayerID))
	{
		float income = Economy.FoodIncome.FindRef(PlayerID);
		float outcome = Economy.FoodUpkeep.FindRef(PlayerID);
		float revenue = income - outcome;
		return revenue;
	}
	else {return 0.0f;}
}

void UEconomyComponent::EndTurnFunction()
{
 //This is where anything to do with the economy at the end of a turn happens
	UE_LOG(LogTemp,Warning,TEXT(" EconComp EndTurn Function Called"));
	if(GameModeRef)
	{
		AddGold(0,GetPlayerGoldRevenue(0));
		AddFood(0,GetPlayerFoodRevenue(0));
		AddGold(1,GetPlayerGoldRevenue(1));
		AddFood(1,GetPlayerFoodRevenue(1));
		AddGold(2,GetPlayerGoldRevenue(2));
		AddFood(2,GetPlayerFoodRevenue(2));
		//UE_LOG(LogTemp,Warning,TEXT("Gold Revenue: %f"),GetPlayerGoldRevenue(0));
	}
	
	
}

