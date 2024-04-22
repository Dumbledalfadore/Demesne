// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "UI/UnitCardWidget.h"
#include "UnitCardButtonWidget.generated.h"

UCLASS()
class DEMESNE_API UUnitCardButtonWidget : public UUnitCardWidget
{
	GENERATED_BODY()

public:
	FOnSlotButtonDelegate OnClickedDelegate;

	UFUNCTION()
	void SetCanAfford(const bool State);

protected:
	UPROPERTY(meta=(BindWidget))
	class UImage* CanAffordIcon;

	virtual void OnButtonClicked() override;
};
