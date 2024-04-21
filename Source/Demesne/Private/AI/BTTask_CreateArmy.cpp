// Created by WarRoom Wizards ©2023 - 2024


#include "AI/BTTask_CreateArmy.h"

#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Military/ArmyManager.h"
#include "Military/ArmyUnit.h"
#include "Military/UnitData.h"
#include "Settlements/Settlement.h"
#include "Settlements/SettlementManager.h"

UBTTask_CreateArmy::UBTTask_CreateArmy(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Create Army";
}

EBTNodeResult::Type UBTTask_CreateArmy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(Npc->GetClass()->IsChildOf(AStrategyAIPawn::StaticClass()))
			{
				
				if(auto* const GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					float Gold = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("Gold"));
					
					int PlayerID = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PlayerID"));
					//Get the AI's main settlement and tile index
					ASettlement* settlement = GM->SettlementManager->GetSettlementsByID(PlayerID)[0];
					auto TileIndex = settlement->GetTileIndex();
					//Get Units that the settlement can build
					TArray<TSubclassOf<AArmyUnit>> UnitArr = settlement->GetSpawnableUnits();
					
					//Get Random buildable unit
					const int32 Index = FMath::RandRange(0, UnitArr.Num() - 1);
					TSubclassOf<AArmyUnit> UnitToSpawn = UnitArr[Index];
					
					if(UnitToSpawn != nullptr)
					{
						//Check if the AI has enough Gold
						float UnitGold = UnitToSpawn.GetDefaultObject()->GetDefaultUnitData()->GetGoldCostToCreate();
						if(UnitGold < Gold)
						{
							//if we have enough Gold, spawn army at city.
							GM->ArmyManager->CreateArmy(TileIndex,UnitToSpawn,PlayerID);
						}
					}
					
					
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
	
}
