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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settlements|UI")
	UUserWidget* SettlementUI;

	UPROPERTY(EditAnywhere, Category = "Settlements|UI")
	TSubclassOf<class UUserWidget> SettlementUIClass;

	UPROPERTY(BlueprintAssignable, Category = "Settlements|UI|Event")
	FSettlementUpdated OnSettlementUpdated;

private:
	UFUNCTION()
	void UpdateSettlementUI(ASettlement* Settlement);

	UFUNCTION()
	void SetSettlementUIVisibility(bool Visible);

};
