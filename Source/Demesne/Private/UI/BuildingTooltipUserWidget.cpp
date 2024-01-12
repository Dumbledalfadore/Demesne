// Created by WarRoom Wizards ©2023


#include "UI/BuildingTooltipUserWidget.h"

#include "BuildingData.h"
#include "Components/TextBlock.h"

void UBuildingTooltipUserWidget::UpdateTooltip(UBuildingData* Building)
{
	BuildingNameText->SetText(FText::FromString(Building->BuildingName));
}
