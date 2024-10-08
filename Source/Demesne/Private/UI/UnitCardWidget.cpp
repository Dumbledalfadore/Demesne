// Created by WarRoom Wizards ©2023


#include "UI/UnitCardWidget.h"

#include "Components/Button.h"

void UUnitCardWidget::UpdateUnitCard(UUnitData* UnitData)
{
	if(!UnitData) return;
	
	SetUnitImage(UnitData->GetIcon());
}

void UUnitCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UnitCardButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
	UnitCardButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnButtonHovered);
}

void UUnitCardWidget::NativeDestruct()
{
	Super::NativeDestruct();

		UnitCardButton->OnClicked.RemoveDynamic(this, &ThisClass::OnButtonClicked);
	UnitCardButton->OnHovered.RemoveDynamic(this, &ThisClass::OnButtonHovered);
}

void UUnitCardWidget::OnButtonClicked()
{
}

void UUnitCardWidget::OnButtonHovered()
{
	OnHoveredDelegate.Broadcast(Index);
}
