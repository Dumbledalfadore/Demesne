// Created by WarRoom Wizards ©2023


#include "UI/UnitCardButtonWidget.h"

#include "Components/Button.h"

void UUnitCardButtonWidget::SetCanAfford(const bool State)
{
	if(State)
	{
		UnitCardButton->SetIsEnabled(true);
		CanAffordIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UnitCardButton->SetIsEnabled(false);
		CanAffordIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUnitCardButtonWidget::OnButtonClicked()
{
	OnClickedDelegate.Broadcast(Index);
}
