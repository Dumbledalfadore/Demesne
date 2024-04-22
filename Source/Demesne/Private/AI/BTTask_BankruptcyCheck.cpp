// Created by WarRoom Wizards ©2023


#include "AI/BTTask_BankruptcyCheck.h"

#include "StrategyLayerGameMode.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BankruptcyCheck::UBTTask_BankruptcyCheck(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Check for Bankruptcy";
}

EBTNodeResult::Type UBTTask_BankruptcyCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
				if(auto* const GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					UE_LOG(LogTemp,Warning,TEXT("BankruptGm"));
					//gbankrupt if  gold =is less than 0 and gold + (income - upkeep) is less than 0.
					//Basically if the AI wont be on a positive amount of gold next turn it is bankrupt
					float Gold = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("Gold"));
					float Income = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("GoldIncome"));
					float Upkeep = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("GoldUpkeep"));
					if(Gold < 0.f && Gold + (Income - Upkeep)< 0.f)
					{
						UE_LOG(LogTemp,Warning,TEXT("is bankrupt"));
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsBankrupt"),true);
					}
					else
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsBankrupt"),false);
					}
					return EBTNodeResult::Failed;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
