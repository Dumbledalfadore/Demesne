// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettlementUserWidget.generated.h"

class AArmyUnit;
class UUnitData;
enum class EBuildingType : uint8;
/**
 * 
 */
UCLASS()
class DEMESNE_API USettlementUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Set from controller when the settlement is clicked */
	UPROPERTY(BlueprintReadOnly)
	class AActor* Owner;
	
protected:
	/* UI Binds */ ////////////////////////////////////////////////////////////////////
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SettlementNameText;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* CurrentBuildingWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* BuildingOptionsWrapBox;
	
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* GarrisonWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* ArmyWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UOverlay* BuildingOptionsOverlay;

	UPROPERTY(meta=(BindWidget))
	class UButton* BuildingOverlayButton;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* SettlementButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* GarrisonButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* CreateArmyButton;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* MainWidgetSwitcher;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* SettlementWidgetSwitcher;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PopulationText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* FoodText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta=(BindWidget))
	class UOverlay* NotificationOverlay;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NotificationText;
	//////////////////////////////////////////////////////////////////////////////////
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	/* Updates the UI */
	void OnSettlementUpdated();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta=(ExposeOnSpawn))
	TSubclassOf<class UBuildingTooltipUserWidget> BuildingTooltipClass;

	UPROPERTY()
	class UBuildingTooltipUserWidget* BuildingTooltip;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* BuildIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ExpandIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta=(ExposeOnSpawn))
	TSubclassOf<class USettlementButtonUserWidget> BuildingSlotButtonClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<class USettlementButtonUserWidget*> MainBuildingButtons;

	UPROPERTY(BlueprintReadWrite)
	TArray<class USettlementButtonUserWidget*> SubBuildingButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta=(ExposeOnSpawn))
	TSubclassOf<class UUnitCardWidget> UnitCardWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta=(ExposeOnSpawn))
	TSubclassOf<class UUnitCardButtonWidget> UnitCardButtonWidgetClass;

	UPROPERTY()
	TArray<class UUnitCardWidget*> UnitCardWidgets;
	
	UPROPERTY()
	TArray<class UUnitCardButtonWidget*> UnitCardButtonWidgets;

	UPROPERTY()
	TMap<EBuildingType, FLinearColor> BackgroundColours;

	UFUNCTION()
	void AddBackgroundColour(EBuildingType Type, FLinearColor Colour);

	UFUNCTION()
	FLinearColor GetBackgroundColour(EBuildingType Type); 
	
	UPROPERTY()
	class ADemesnePlayerController* Controller;

	UPROPERTY()
	class ASettlement* Settlement;

	/* Keeps track of which slot we want to build in */
	UPROPERTY()
	int CurrentSlotIndex;

	/* Holds what buildings can currently be built in the slot */
	UPROPERTY()
	TArray<class UBuildingData*> BuildingsToBuild;
		
	UFUNCTION()
	void CreateButtons(TArray<USettlementButtonUserWidget*>& ButtonArray, UWrapBox* WrapBox, int Count, bool IsBaseSlot);
	
	UFUNCTION()
	void UpdateButtons(int Count);

	UFUNCTION()
	void UpdateButtonUIs(TArray<USettlementButtonUserWidget*> ButtonArray, TArray<class UBuildingData*> BuildingDataArray, FString ButtonType);

	UFUNCTION()
	void UpdateGarrison();
			
	/* Updates the gold/food UI */
	UFUNCTION()
	void UpdateResourceValues();

	UFUNCTION()
	void OnNotification(FString Notification);

	UFUNCTION()
	void OnNotificationStop();

	UFUNCTION()
	void SwitchToSettlement();

	UFUNCTION()
	void SwitchToGarrison();

	UFUNCTION()
	void SwitchToArmy();

	UPROPERTY(EditAnywhere)
	float NotificationTimer;

	FTimerHandle NotificationTimerHandle;

	UFUNCTION()
	void OnSlotButtonClicked(int Index);

	UFUNCTION()
	void OnBuildButtonClicked(int Index);

	UFUNCTION()
	void OnSlotButtonHovered(int Index);

	UFUNCTION()
	void OnBuildButtonHovered(int Index);

	UFUNCTION()
	void OnUnitCardButtonClicked(int Index);

	UPROPERTY()
	class AStrategyLayerGameMode* GM;
};
