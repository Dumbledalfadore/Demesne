// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingTooltipUserWidget.generated.h"

class UUnitData;
class AArmyUnit;
/**
 * 
 */
UCLASS()
class DEMESNE_API UBuildingTooltipUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void UpdateTooltipBuilding(class UBuildingData* Building, bool bIsBuilt);

	UFUNCTION()
	void UpdateTooltipArmy(AArmyUnit* Army, bool bIsSpawned);

	UFUNCTION()
	void UpdateTooltipUnit(UUnitData* Unit, bool bIsSpawned, bool bIsGarrisoned);

	UFUNCTION()
	void UpdateTooltipForced(FString Title, FString Description);

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TitleText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TierText;

	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* BuildingDescriptionText;

	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* CostDescriptionText;

	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* TurnCostDescriptionText;
	
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* DescriptionLayout;

	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* CostLayout;

	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* OngoingLayout;
};
