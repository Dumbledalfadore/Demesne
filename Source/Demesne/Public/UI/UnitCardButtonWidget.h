// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "UI/UnitCardWidget.h"
#include "UnitCardButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotButtonDelegate, int, Index);

UCLASS()
class DEMESNE_API UUnitCardButtonWidget : public UUnitCardWidget
{
	GENERATED_BODY()

public:
	FOnSlotButtonDelegate OnClickedDelegate;
	FOnSlotButtonDelegate OnHoveredDelegate;

protected:
	UPROPERTY(meta=(BindWidget))
	class UButton* UnitCardButton;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnButtonClicked();
	
};
