// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTAsk_CheckEconomy.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API UBTTAsk_CheckEconomy : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTAsk_CheckEconomy(FObjectInitializer const& FObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI", meta = (AllowPrivateAccess = true))
	int PlayerID = 1;
};
