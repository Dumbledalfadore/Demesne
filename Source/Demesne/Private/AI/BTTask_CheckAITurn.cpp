// Created by WarRoom Wizards ©2023 - 2024


#include "AI/BTTask_CheckAITurn.h"

#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_CheckAITurn::UBTTask_CheckAITurn(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Check if AI Turn";
}

EBTNodeResult::Type UBTTask_CheckAITurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(Npc->GetClass()->IsChildOf(AStrategyAIPawn::StaticClass()))
			{
				AStrategyAIPawn* AIPawn = Cast<AStrategyAIPawn>(Npc);
				
				if(auto* const GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
				
					
					if(GM->TurnManager->TurnState == ECOMPUTERTURN)
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsTurn"),true);
						//UE_LOG(LogTemp,Warning,TEXT("AITurn"));
					}
					else
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsTurn"),false);
						//UE_LOG(LogTemp,Warning,TEXT("PC Turn"));
					}
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
	
}
