// Created by WarRoom Wizards ©2023 - 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverviewPanelWidget.generated.h"

class UOverviewListWidget;
class AStrategyLayerGameMode;
/**
 * 
 */
UCLASS()
class DEMESNE_API UOverviewPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UOverlay* MainOverlay;

	UPROPERTY(meta=(BindWidget))
	class UOverlay* MainListOverlay;
	
	UPROPERTY(meta=(BindWidget))
	class UOverlay* SettlementsOverlay;

	UPROPERTY(meta=(BindWidget))
	class UOverlay* ArmiesOverlay;

	UPROPERTY(meta=(BindWidget))
	class UScrollBox* MainListSB;

	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SettlementListSB;

	UPROPERTY(meta=(BindWidget))
	class UScrollBox* ArmyListSB;

	UPROPERTY(meta=(BindWidget))
	class UButton* OpenCloseButton;

	UPROPERTY(meta=(BindWidget))
	class UImage* OpenCloseButtonIcon;

	UPROPERTY(meta=(BindWidget))
	class UButton* OverviewButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* SettlementsButton;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* ArmiesButton;
	
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* ListSwitcher;

	UFUNCTION()
	void RefreshSettlementUI();

	UFUNCTION()
	void RefreshArmyUI();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	AStrategyLayerGameMode* GM;

	UPROPERTY()
	class ADemesnePlayerController* Controller;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverviewListWidget> SettlementWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverviewListWidget> ArmyWidgetClass;

	UPROPERTY()
	TArray<UOverviewListWidget*> AllSettlementListWidgets;
	
	UPROPERTY()
	TArray<UOverviewListWidget*> SettlementListWidgets;

	UPROPERTY()
	TArray<UOverviewListWidget*> ArmyListWidgets;

	UFUNCTION()
	void SwitchWidgetAllSettlements();

	UFUNCTION()
	void SwitchWidgetSettlements();

	UFUNCTION()
	void SwitchWidgetArmies();

	UFUNCTION()
	void OpenClose();
};
