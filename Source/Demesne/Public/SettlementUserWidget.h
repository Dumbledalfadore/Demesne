// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/SettlementButtonUserWidget.h"
#include "SettlementUserWidget.generated.h"

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
	/* UI */ /////////////////////////////////////////////////////////////////////////
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SettlementNameText;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* CurrentBuildingWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* BuildingOptionsWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UOverlay* BuildingOptionsOverlay;

	UPROPERTY(meta=(BindWidget))
	class UButton* BuildingOverlayButton;
	//////////////////////////////////////////////////////////////////////////////////
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	/* Updates the UI */
	void OnSettlementUpdated();
	
	/* Called whenever the turn ends for if we need to update anything
	 * TODO: Create delegate to link to turn manager
	 */
	void OnNextTurn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* BuildIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ExpandIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta=(ExposeOnSpawn))
	TSubclassOf<USettlementButtonUserWidget> BuildingSlotButtonClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<class USettlementButtonUserWidget*> MainBuildingButtons;

	UPROPERTY(BlueprintReadWrite)
	TArray<class USettlementButtonUserWidget*> SubBuildingButtons;

	UPROPERTY()
	TMap<EBuildingType, FLinearColor> BackgroundColours;
	
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

	UFUNCTION(BlueprintCallable)
	FString GetBuildingCountAsString();

	UFUNCTION()
	FLinearColor GetBackgroundColour(EBuildingType Type);

	UFUNCTION()
	void CreateButtons(TArray<USettlementButtonUserWidget*>& ButtonArray, UWrapBox* WrapBox, int Count, bool IsBaseSlot);
	
	UFUNCTION()
	void UpdateButtons(int Count);

	UFUNCTION()
	void UpdateButtonUIs(TArray<USettlementButtonUserWidget*> ButtonArray, TArray<class UBuildingData*> BuildingDataArray);

	UFUNCTION()
	void OnSlotButtonClicked(int Index);

	UFUNCTION()
	void OnBuildButtonClicked(int Index);
};
