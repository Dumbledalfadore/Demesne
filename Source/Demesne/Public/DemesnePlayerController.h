// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Military/ArmyUnit.h"
#include "DemesnePlayerController.generated.h"

class AArmyUnit;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClickUpdated);

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RightlickInput;

protected:
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void OnLeftClick(const FInputActionValue& Value);
	void OnRightClick(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Demesne|Settlements|UI")
	UUserWidget* SettlementUI;

	UPROPERTY(EditAnywhere, Category = "Demesne|Settlements|UI")
	TSubclassOf<class UUserWidget> SettlementUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Demesne|Army|UI")
	UUserWidget* ArmyUI;

	UPROPERTY(EditAnywhere, Category = "Demesne|Army|UI")
	TSubclassOf<class UUserWidget> ArmyUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Demesne|Army|UI")
	UUserWidget* BattleReportUI;

	UPROPERTY(EditAnywhere, Category = "Demesne|Army|UI")
	TSubclassOf<class UUserWidget> BattleReportUIClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Demesne|UI")
	UUserWidget* StrategyUI;

	UPROPERTY(EditAnywhere, Category = "Demesne|UI")
	TSubclassOf<class UUserWidget> StrategyUIClass;

	UPROPERTY(BlueprintAssignable, Category = "Demesne|Settlements|UI|Event")
	FClickUpdated OnSettlementUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Demesne|Army|UI|Event")
	FClickUpdated OnArmyUpdated;

private:
	UFUNCTION()
	void UpdateSettlementUI(ASettlement* Settlement);

	UFUNCTION()
	void SetSettlementUIVisibility(bool Visible);

	UFUNCTION()
	void UpdateArmyUI(AArmyUnit* Army);

	UFUNCTION()
	void SetArmyUIVisibility(bool Visible);

	UFUNCTION()
	void UpdateBattleReportUI(UArmyDataComponent* Aggressor, UArmyDataComponent* Defender, FString Victor);

	UFUNCTION()
	void SetBattleReportUIVisibility(bool Visible);

	UPROPERTY()
	AArmyUnit* CurrentSelectedUnit = nullptr;
};
