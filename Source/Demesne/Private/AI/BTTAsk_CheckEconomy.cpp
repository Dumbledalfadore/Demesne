// Created by WarRoom Wizards ©2023


#include "AI/BTTAsk_CheckEconomy.h"

#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "AI/DemesneAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTAsk_CheckEconomy::UBTTAsk_CheckEconomy(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Check AI Currency Balances";
}

EBTNodeResult::Type UBTTAsk_CheckEconomy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(auto* const GM = Cast<AStrategyLayerGameMode>(GetWorld()))
			{
				GM->EconComp->GetGold(PlayerID);
				OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(),GM->EconComp->GetGold(PlayerID));
			}
			return EBTNodeResult::Succeeded;
		}
			
	}
	return EBTNodeResult::Failed;
}
