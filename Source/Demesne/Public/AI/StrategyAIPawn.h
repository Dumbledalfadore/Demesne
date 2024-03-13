// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "StrategyAIPawn.generated.h"

UCLASS()
class DEMESNE_API AStrategyAIPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStrategyAIPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
    int PlayerID;
	
	UBehaviorTree* GetBehaviourTree() const;//BT getter for AI Controller
	
};
