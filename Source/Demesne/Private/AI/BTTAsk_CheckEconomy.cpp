// Created by WarRoom Wizards ©2023


#include "AI/BTTAsk_CheckEconomy.h"

#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTAsk_CheckEconomy::UBTTAsk_CheckEconomy(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Check AI Currency Balances";
}

EBTNodeResult::Type UBTTAsk_CheckEconomy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*If we have a controller, and the controller is controlling a pawn that is the Strategy AI pawn, set the Player ID and set all required Economy keys*/
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(Npc->GetClass()->IsChildOf(AStrategyAIPawn::StaticClass()))
			{
				AStrategyAIPawn* AIPawn = Cast<AStrategyAIPawn>(Npc);
				PlayerID = AIPawn->PlayerID;
				if(auto* const GM = Cast<AStrategyLayerGameMode>(GetWorld()))
				{
					GM->EconComp->GetGold(PlayerID);
					GM->EconComp->GetFood(PlayerID);
					GM->EconComp->GetGoldUpkeep(PlayerID);
					GM->EconComp->GetFoodUpkeep(PlayerID);
					GM->EconComp->GetGoldIncome(PlayerID);
					GM->EconComp->GetFoodIncome(PlayerID);
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("Gold"),GM->EconComp->GetGold(PlayerID));
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("Food"),GM->EconComp->GetFood(PlayerID));
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("GoldUpkeep"),GM->EconComp->GetGoldUpkeep(PlayerID));
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("FoodUpkeep"),GM->EconComp->GetFoodUpkeep(PlayerID));
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("GoldIncome"),GM->EconComp->GetGoldIncome(PlayerID));
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("FoodIncome"),GM->EconComp->GetFoodIncome(PlayerID));
					OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("PlayerID"),PlayerID);
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
