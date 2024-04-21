// Created by WarRoom Wizards ©2023 - 2024


#include "AI/BTTask_StopRun.h"

#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_StopRun::UBTTask_StopRun(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Stop Tree";
}

EBTNodeResult::Type UBTTask_StopRun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(Npc->GetClass()->IsChildOf(AStrategyAIPawn::StaticClass()))
			{
				
				
					
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bShouldRun"),false);
				
				return EBTNodeResult::Succeeded;
				
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
