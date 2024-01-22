// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "SettlementButtonUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonDelegate, int, Index);

UCLASS()
class DEMESNE_API USettlementButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Index set when created
	 * Used for adding buildings to the correct array slot within CurrentBuildings in Settlement */
	UPROPERTY()
	int Index;

	FOnButtonDelegate OnClickedDelegate;
	FOnButtonDelegate OnHoveredDelegate;
	
	UFUNCTION()
	void SetIcon(UTexture2D* NewIcon);

	UFUNCTION()
	void SetBackground(FLinearColor Colour);

	UFUNCTION()
	void SetTier(FString Tier);

	UFUNCTION()
	void SetPopulation(FString Population);

	UFUNCTION()
	void SetPopulationVisible(ESlateVisibility Visible);

	UFUNCTION()
	void SetButtonEnabled(const bool bEnabled) const { SlotButton->SetIsEnabled(bEnabled); }

	UFUNCTION()
	void SetIsLocked(bool bIsLocked);
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UOverlay* PopulationOverlay;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PopulationText;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* LockedIcon;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* BuildingIcon;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* BuildingTierText;

	UPROPERTY(meta=(BindWidget))
	class UButton* SlotButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* HiddenButton;

	UFUNCTION()
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonHovered();
};
