// Created by WarRoom Wizards ©2023


#include "AI/BTTask_UpgradeSettlements.h"

#include "StrategyLayerGameMode.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Settlements/SettlementManager.h"

UBTTask_UpgradeSettlements::UBTTask_UpgradeSettlements(FObjectInitializer const& FObjectInitializer)
{
 NodeName = "Upgrade all settlements";
}

EBTNodeResult::Type UBTTask_UpgradeSettlements::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(Npc->GetClass()->IsChildOf(AStrategyAIPawn::StaticClass()))
			{
				AStrategyAIPawn* AIPawn = Cast<AStrategyAIPawn>(Npc);
				//get player id
				int ID = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PlayerID"));
				if(auto* const GM = Cast<AStrategyLayerGameMode>(GetWorld()))
				{
					//get all settlements with player ID
					SettlementArr = GM->SM->GetSettlementsByID(ID);
					ASettlement* settlement;
					// for each settlement in array call the AI upgrade function
					for(int i = 0; i <SettlementArr.Num();i++)
					{
						
						settlement = SettlementArr[i];
						settlement->AITryRandomSettlementModification();
						
					}
					
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
