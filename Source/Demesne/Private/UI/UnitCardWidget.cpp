// Created by WarRoom Wizards ©2023


#include "UI/UnitCardWidget.h"

void UUnitCardWidget::UpdateUnitCard(UUnitData* UnitData)
{
	if(!UnitData) return;
	
	SetUnitImage(UnitData->GetIcon());
}
