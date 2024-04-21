// Created by WarRoom Wizards ©2023 - 2024


#include "AI/BTTask_Move.h"

#include "GridManager.h"
#include "StrategyLayerGameMode.h"
#include "Tile.h"
#include "AI/DemesneAIController.h"
#include "AI/StrategyAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Military/ArmyManager.h"
#include "Utils/Pathfinder.h"

class AArmyUnit;

UBTTask_Move::UBTTask_Move(FObjectInitializer const& FObjectInitializer)
{
	NodeName = "Move/Attack Units";
}

EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ADemesneAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto* const Npc = Controller->GetPawn())
		{
			if(Npc->GetClass()->IsChildOf(AStrategyAIPawn::StaticClass()))
			{
				
				if(auto* const GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					AGridManager* GridM = GM->GridManager;
					bool bBattleFound = false;
					int PlayerID = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PlayerID"));
					
					//Get all units that belong to AI
					TArray<AArmyUnit*> UnitArr = GM->ArmyManager->GetArmiesByID(PlayerID);
					TArray<AArmyUnit*> EnemyArr;
					//Get Number of players and iterate through the playerIDs. Append armies of the ID to enemy array if it does not match the AI's Player ID
					int PlayerNum = GM->NumberofPlayers;
					for(int i =0; i <= PlayerNum;i++)
					{
						if(i != PlayerID) EnemyArr.Append(GM->ArmyManager->GetArmiesByID(i));
					}
					//Get All Tiles very inefficient change if possible
					TArray<ATile*> TileArr;
					TArray<ATile*> ReachableArr;
					TArray<AActor*> FoundTiles;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATile::StaticClass(),FoundTiles);
					for(AActor* A : FoundTiles){TileArr.Add(Cast<ATile>(A));}
					if(GridM)
					{
						for(int i = 0; i <= UnitArr.Num() - 1;i++)
						{
							//get the units location
							ATile* UnitPos = GridM->GetTile(UnitArr[i]->GetTileIndex());
							FIntPoint TileLocation = TileArr[i]->TileIndex;
							//check if within remaining movement distance
							if(APathfinder::Distance(UnitPos,TileArr[i]) <= UnitArr[i]->GetRemainingMovementDistance() )
							{
								//Add to reachable tiles
								ReachableArr.Add(TileArr[i]);
							}
							//get random reachable tile index
							const int32 RandTile = FMath::RandRange(0, ReachableArr.Num() - 1);
							//For each unit, iterate through all enemy armies, if an army has the same tile index as one of the reachable tiles, initiate battle.
							//if no battles found move to random location
							
							for(int j = 0; j <= EnemyArr.Num()-1; j++)
							{
								for(int k = 0; k <= ReachableArr.Num() - 1; k++)
								{
									if(EnemyArr[j]->GetTileIndex() == ReachableArr[k]->TileIndex)
									{
										GM->ArmyManager->InitiateBattle(UnitArr[i]->GetArmyComponent(),EnemyArr[j]->GetArmyComponent());
										bBattleFound = true;
									}
								}
							}
							if(!bBattleFound) UnitArr[i]->SetActorLocation(ReachableArr[RandTile]->GetActorLocation() + FVector(0.f,0.f,90.f));
							
						}
					
						
						for(int i =0; i <= UnitArr.Num() - 1; i++)
						{
							
							
							
						}
					}
					
					
					
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
