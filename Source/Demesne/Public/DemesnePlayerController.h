// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "DemesnePlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSettlementUpdated);

class UInputAction;

UCLASS()
class DEMESNE_API ADemesnePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADemesnePlayerController();
	
	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/* Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftClickInput;

protected:
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void OnLeftClick(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Demesne|Settlements|UI")
	UUserWidget* SettlementUI;

	UPROPERTY(EditAnywhere, Category = "Demesne|Settlements|UI")
	TSubclassOf<class UUserWidget> SettlementUIClass;

	UPROPERTY(EditAnywhere, Category = "Demesne|UI")
	TSubclassOf<class ATurnManager> TurnManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Demesne|UI")
	UUserWidget* StrategyUI;

	UPROPERTY(EditAnywhere, Category = "Demesne|UI")
	TSubclassOf<class UUserWidget> StrategyUIClass;

	UPROPERTY(BlueprintAssignable, Category = "Demesne|Settlements|UI|Event")
	FSettlementUpdated OnSettlementUpdated;

private:
	UFUNCTION()
	void UpdateSettlementUI(ASettlement* Settlement);

	UFUNCTION()
	void SetSettlementUIVisibility(bool Visible);

};
