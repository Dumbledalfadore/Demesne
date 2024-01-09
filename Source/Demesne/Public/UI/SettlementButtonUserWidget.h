// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettlementButtonUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClickedDelegate, int, Index);

UCLASS()
class DEMESNE_API USettlementButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Index set when created
	 * Used for adding buildings to the correct array slot within CurrentBuildings in Settlement */
	UPROPERTY()
	int Index;

	FOnButtonClickedDelegate OnClickedDelegate;
	
	UFUNCTION()
	void SetIcon(UTexture2D* NewIcon);

	UFUNCTION()
	void SetBackground(FLinearColor Colour);

	UFUNCTION()
	void SetTier(FString Tier);
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UImage* BuildingIcon;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* BuildingTierText;

	UPROPERTY(meta=(BindWidget))
	class UButton* SlotButton;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void OnButtonClicked();
};
