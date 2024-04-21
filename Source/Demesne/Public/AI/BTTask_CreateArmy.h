// Created by WarRoom Wizards ©2023 - 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CreateArmy.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API UBTTask_CreateArmy : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_CreateArmy(FObjectInitializer const& FObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
