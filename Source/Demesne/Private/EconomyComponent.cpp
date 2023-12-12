// Fill out your copyright notice in the Description page of Project Settings.
#include "EconomyComponent.h"
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
	
}


// Called every frame
void UEconomyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEconomyComponent::UpdateIncome()
{
	//will require the turn system to actually be done before i can do this
	if(GameModeRef)
	{
		//updates both so we can have building that produce both or produce on at the cost of another
		// e.g a sheep farm (mutton = +food, wool = +money) or an Inn (+Money, --Food)
		//ensure only one of the fields are filled for the mono currency buildings (ie. DPT = 5.0f, FPT = 0.0f)
		GameModeRef->UpdateMoney(Dpt);
		GameModeRef->UpdateFood(Fpt);
	}
		
}

