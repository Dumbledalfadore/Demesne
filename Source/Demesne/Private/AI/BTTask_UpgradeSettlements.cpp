// Created by WarRoom Wizards ©2023


#include "AI/BTTask_UpgradeSettlements.h"

#include "StrategyLayerGameMode.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
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
				UE_LOG(LogTemp,Warning,TEXT("GotPawn"));
				AStrategyAIPawn* AIPawn = Cast<AStrategyAIPawn>(Npc);
				//get player id
				int ID = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PlayerID"));
				if(auto* const GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					UE_LOG(LogTemp,Warning,TEXT("Got GM"));
					//get all settlements with player ID
					SettlementArr = GM->SettlementManager->GetSettlementsByID(ID);
					ASettlement* Settlement;
					// for each settlement in array call the AI upgrade function
					for(int i = 0; i <= SettlementArr.Num()-1;i++)
					{
						
						Settlement = SettlementArr[i];
						Settlement->AITryRandomSettlementModification();
						UE_LOG(LogTemp,Warning,TEXT("Upgrading"));
					}
					
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
