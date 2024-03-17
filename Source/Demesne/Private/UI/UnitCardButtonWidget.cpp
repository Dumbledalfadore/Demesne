// Created by WarRoom Wizards ©2023


#include "UI/UnitCardButtonWidget.h"

#include "Components/Button.h"

void UUnitCardButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UnitCardButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
}

void UUnitCardButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UnitCardButton->OnClicked.RemoveDynamic(this, &ThisClass::OnButtonClicked);
}

void UUnitCardButtonWidget::OnButtonClicked()
{
	OnClickedDelegate.Broadcast(Index);
}
