// Created by WarRoom Wizards ©2023 - 2024


#include "AI/BTTask_ShouldRun.h"

#include "StrategyLayerGameMode.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"

class AStrategyAIPawn;

UBTTask_ShouldRun::UBTTask_ShouldRun(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Should Run?";
}

EBTNodeResult::Type UBTTask_ShouldRun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(Npc->GetClass()->IsChildOf(AStrategyAIPawn::StaticClass()))
			{
				
				
					
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bShouldRun"),true);
				
					return EBTNodeResult::Succeeded;
				
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
