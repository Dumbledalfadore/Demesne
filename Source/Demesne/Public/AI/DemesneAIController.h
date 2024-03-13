// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DemesneAIController.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API ADemesneAIController : public AAIController
{
	GENERATED_BODY()
public:
	explicit ADemesneAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
