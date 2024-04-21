// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArmyUserWidget.generated.h"



UCLASS()
class DEMESNE_API UArmyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	class AActor* Owner;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/* Updates the UI */
	UFUNCTION()
	void OnArmyUpdated();

	UFUNCTION()
	void UpdatedArmyCards();

	UFUNCTION()
	void ShowRecruits();

	UFUNCTION()
	void RecruitUnit(int Index);

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ArmyNameText;

	UPROPERTY(meta=(BindWidget))
	class UOverlay* RecruitArmyOverlay;
	
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* ArmyWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* RecruitWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UButton* RecruitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TSubclassOf<class UBuildingTooltipUserWidget> ArmyTooltipClass;

	UPROPERTY()
	class UBuildingTooltipUserWidget* ArmyTooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TSubclassOf<class UUnitCardButtonWidget> UnitCardButtonWidgetClass;

	/* Army Unit Cards */
	UPROPERTY()
	TArray<class UUnitCardButtonWidget*> UnitCardButtonWidgets;

	/* Recruit Unit Cards */
	UPROPERTY()
	TArray<class UUnitCardButtonWidget*> RecruitUnitCardButtonWidgets;

	UPROPERTY()
	class ADemesnePlayerController* Controller;

	/* Currently Selected Army */
	UPROPERTY()
	class AArmyUnit* Army;

	UPROPERTY()
	class AStrategyLayerGameMode* GM;

	UFUNCTION()
	void OnUnitCardArmyHovered(int Index);

	UFUNCTION()
	void OnUnitCardRecruitHovered(int Index);
};
