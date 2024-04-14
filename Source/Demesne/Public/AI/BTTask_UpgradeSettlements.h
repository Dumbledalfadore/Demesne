// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Settlements/Settlement.h"
#include "BTTask_UpgradeSettlements.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API UBTTask_UpgradeSettlements : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_UpgradeSettlements(FObjectInitializer const& FObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	TArray<ASettlement*> SettlementArr;
};
