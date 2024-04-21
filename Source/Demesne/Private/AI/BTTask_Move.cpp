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
					//UE_LOG(LogTemp,Warning,TEXT("GotGameModeMove"));
					AGridManager* GridM = GM->GridManager;
					APathfinder* Pathfinder = GM->PathFinder;
					//if(Pathfinder){UE_LOG(LogTemp,Warning,TEXT("GotPathfinder"));}
					bool bBattleFound = false;
					int PlayerID = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PlayerID"));
					
					//Get all units that belong to AI
					TArray<AArmyUnit*> UnitArr = GM->ArmyManager->GetArmiesByID(PlayerID);
					TArray<AArmyUnit*> EnemyArr;
					//Get Number of players and iterate through the playerIDs. Append armies of the ID to enemy array if it does not match the AI's Player ID
					int PlayerNum = GM->NumberofPlayers;
					//UE_LOG(LogTemp,Warning,TEXT("Get PlayerNum %d"),PlayerNum);
					for(int i =0; i <= PlayerNum - 1;i++)
					{
						//UE_LOG(LogTemp,Warning,TEXT("PlayerNum"));
						if(i != PlayerID) EnemyArr.Append(GM->ArmyManager->GetArmiesByID(i));
					}
					//Get All Tiles very inefficient change if possible
					TArray<ATile*> TileArr;
					TArray<ATile*> ReachableArr;
					TArray<AActor*> FoundTiles;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATile::StaticClass(),FoundTiles);
					for(AActor* A : FoundTiles){TileArr.Add(Cast<ATile>(A));}
					
					UE_LOG(LogTemp,Warning,TEXT("Tiles Found: %d"),TileArr.Num());
					if(GridM)
					{
						//UE_LOG(LogTemp,Warning,TEXT("GridManager Loaded"));
						UE_LOG(LogTemp,Warning,TEXT("UnitArr: %d"),UnitArr.Num());
						for(int i = 0; i <= UnitArr.Num() - 1;i++)
						{
							//get the units location
							ATile* UnitPos = GridM->GetTile(UnitArr[i]->GetTileIndex());
							UE_LOG(LogTemp,Warning,TEXT("Distance: %f"),Pathfinder->Distance(UnitPos,TileArr[i]));
							//check if within remaining movement distance
							for(int b = 0; b <= TileArr.Num() - 1;b++)
							{
								if(Pathfinder->Distance(UnitPos,TileArr[b]) <= UnitArr[i]->GetRemainingMovementDistance())
								{
									//Add to reachable tiles
									ReachableArr.Add(TileArr[b]);
									//UE_LOG(LogTemp,Warning,TEXT("Reachable Tiles"));
								}
								else
								{
									//UE_LOG(LogTemp,Warning,TEXT("Reachable Tile ERROR"));
								}
							}
							
							//get random reachable tile index
							const int32 RandTile = FMath::RandRange(0, ReachableArr.Num() - 1);
							//For each unit, iterate through all enemy armies, if an army has the same tile index as one of the reachable tiles, initiate battle.
							//if no battles found move to random location
							if(EnemyArr.Num() > 0)
							{
								for(int j = 0; j <= EnemyArr.Num()-1; j++)
								{
									UE_LOG(LogTemp,Warning,TEXT("Enemy Array"));
									for(int k = 0; k <= ReachableArr.Num() - 1; k++)
									{
										UE_LOG(LogTemp,Warning,TEXT("Reachable Array"));
										if(EnemyArr[j]->GetTileIndex() == ReachableArr[k]->TileIndex)
										{
											UE_LOG(LogTemp,Warning,TEXT("Battle Check"));
											GM->ArmyManager->InitiateBattle(UnitArr[i]->GetArmyComponent(),EnemyArr[j]->GetArmyComponent());
											bBattleFound = true;
										}
									}
								}
							}
							UE_LOG(LogTemp,Warning,TEXT("Reachable Tile %d"),ReachableArr.Num());
							UE_LOG(LogTemp,Warning,TEXT("RandTile %d"),RandTile);
							if(!bBattleFound) UE_LOG(LogTemp,Warning,TEXT("Move"));UnitArr[i]->SetActorLocation(ReachableArr[RandTile]->GetActorLocation() + FVector(0.f,0.f,90.f));
							
						}
					
						
					}
					
					
					
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
			
	}
	return EBTNodeResult::Failed;
}
