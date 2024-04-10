// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BankruptcyCheck.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API UBTTask_BankruptcyCheck : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_BankruptcyCheck(FObjectInitializer const& FObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
