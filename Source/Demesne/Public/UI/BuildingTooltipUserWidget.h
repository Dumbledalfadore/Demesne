// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingTooltipUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API UBuildingTooltipUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void UpdateTooltip(class UBuildingData* Building);

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* BuildingNameText;
	
};
