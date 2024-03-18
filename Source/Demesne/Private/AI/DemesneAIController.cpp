// Created by WarRoom Wizards ©2023


#include "AI/DemesneAIController.h"

#include "AI/StrategyAIPawn.h"

ADemesneAIController::ADemesneAIController(FObjectInitializer const& ObjectInitializer)
{
}

void ADemesneAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(AStrategyAIPawn* const npc = Cast<AStrategyAIPawn>(InPawn))
	{
		if(UBehaviorTree* const tree = npc->GetBehaviourTree())
		{
			UBlackboardComponent* Bb;
			//Get the blackboard asset from the tree and assign it as the Blackboard ptr
			UseBlackboard(tree->BlackboardAsset,Bb);
			Blackboard = Bb;
			RunBehaviorTree(tree);
		}
	}
}
