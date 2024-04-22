// Created by WarRoom Wizards ©2023


#include "UI/ArmyUserWidget.h"
#include "DemesnePlayerController.h"
#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "TurnManager.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyDataComponent.h"
#include "Military/ArmyManager.h"
#include "Military/ArmyUnit.h"
#include "UI/BuildingTooltipUserWidget.h"
#include "UI/UnitCardButtonWidget.h"

void UArmyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Controller = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if(Controller)
	{
		Controller->OnArmyUpdated.AddDynamic(this, &ThisClass::OnArmyUpdated);
		RecruitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ShowRecruits);
		GM->TurnManager->OnTurnEnd.AddUniqueDynamic(this, &ThisClass::HideRecruits);
	}

	if (ArmyTooltipClass)
	{
		/* Create the tooltip here so we aren't creating it for each button */
		ArmyTooltip = CreateWidget<UBuildingTooltipUserWidget>(this->GetGameInstance(), ArmyTooltipClass);
	}
}

void UArmyUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if(Controller)
	{
		Controller->OnArmyUpdated.RemoveDynamic(this, &ThisClass::OnArmyUpdated);
		RecruitButton->OnClicked.RemoveDynamic(this, &ThisClass::ShowRecruits);
	}
}

void UArmyUserWidget::OnArmyUpdated()
{
	/* Get the settlement */
	Army = Cast<AArmyUnit>(Owner);

	RecruitArmyOverlay->SetVisibility(ESlateVisibility::Collapsed);
	if(Army)
	{
		if(Army->GetPlayerID()  == 0)
		{
			RecruitButton->SetIsEnabled(true);
		}
		else
		{
			RecruitButton->SetIsEnabled(false);
		}
		
		ArmyNameText->SetText(FText::FromString(Army->GetArmyName()));
		UpdatedArmyCards();
	}
}

void UArmyUserWidget::UpdatedArmyCards()
{
	/* Destroy old cards */
	if(UnitCardButtonWidgets.Num() > 0)
	{
		for(auto Card : UnitCardButtonWidgets)
		{
			Card->OnHoveredDelegate.RemoveDynamic(this, &ThisClass::OnUnitCardArmyHovered);
			Card->RemoveFromParent();
		}

		UnitCardButtonWidgets.Empty();
	}

	auto Units = Army->GetArmyComponent()->GetUnitDataArray();
	if(Units.Num() > 0)
	{
		int Index = 0;
		for(auto Unit : Units)
		{
			if(UnitCardButtonWidgetClass)
			{
				UUnitCardButtonWidget* UnitCardButtonWidget = CreateWidget<UUnitCardButtonWidget>(Controller, UnitCardButtonWidgetClass);
				if(UnitCardButtonWidget)
				{
					UnitCardButtonWidget->SetCanAfford(true);
					UnitCardButtonWidget->SetIndex(Index);
					UnitCardButtonWidget->SetToolTip(ArmyTooltip);
					UnitCardButtonWidgets.Add(UnitCardButtonWidget);
					ArmyWrapBox->AddChild(UnitCardButtonWidget);
					//UE_LOG(LogTemp, Warning, TEXT("ARMY UPDATED"));
					UnitCardButtonWidget->UpdateUnitCard(Unit);

					UnitCardButtonWidget->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::OnUnitCardArmyClicked);
					UnitCardButtonWidget->OnHoveredDelegate.AddUniqueDynamic(this, &ThisClass::OnUnitCardArmyHovered);
				}
			}
			Index++;
		}
	}
}

void UArmyUserWidget::ShowRecruits()
{
	ESlateVisibility Vis = RecruitArmyOverlay->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
	RecruitArmyOverlay->SetVisibility(Vis);

	/* Destroy old cards */
	if(RecruitUnitCardButtonWidgets.Num() > 0)
	{
		for(auto Card : RecruitUnitCardButtonWidgets)
		{
			Card->OnClickedDelegate.RemoveDynamic(this, &ThisClass::RecruitUnit);
			Card->OnHoveredDelegate.RemoveDynamic(this, &ThisClass::OnUnitCardRecruitHovered);
			Card->RemoveFromParent();
		}

		RecruitUnitCardButtonWidgets.Empty();
	}

	int Index = 0;
	for(auto Unit : Army->GetRecruitableUnits())
	{
		if(UnitCardButtonWidgetClass)
		{
			UUnitCardButtonWidget* UnitCardButtonWidget = CreateWidget<UUnitCardButtonWidget>(Controller, UnitCardButtonWidgetClass);
			if(UnitCardButtonWidget)
			{
				UnitCardButtonWidget->SetCanAfford(true);
				if(GM->EconComp)
				{
					bool HasGold = GM->EconComp->GetGold(Army->GetPlayerID()) >= Unit->GetGoldCostToCreate();
					bool HasFood = GM->EconComp->GetFood(Army->GetPlayerID()) >= Unit->GetFoodCostToCreate();

					/* We can't afford this army */
					if(!HasGold || !HasFood)
					{
						UnitCardButtonWidget->SetCanAfford(false);
					}
				}
				RecruitUnitCardButtonWidgets.Add(UnitCardButtonWidget);
				RecruitWrapBox->AddChild(UnitCardButtonWidget);
				UnitCardButtonWidget->SetToolTip(ArmyTooltip);
				UnitCardButtonWidget->SetIndex(Index);
				//UE_LOG(LogTemp, Warning, TEXT("ARMY UPDATED"));
				UnitCardButtonWidget->UpdateUnitCard(Unit);
				UnitCardButtonWidget->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::RecruitUnit);
				UnitCardButtonWidget->OnHoveredDelegate.AddUniqueDynamic(this, &ThisClass::OnUnitCardRecruitHovered);
			}
		}
		Index++;
	}
}

void UArmyUserWidget::HideRecruits()
{
	RecruitArmyOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void UArmyUserWidget::RecruitUnit(int Index)
{
	auto RecruitableUnits = Army->GetRecruitableUnits();
	if(RecruitableUnits[Index])
	{
		Army->GetArmyComponent()->AddUnit(RecruitableUnits[Index]);
		/* Take the cost for the unit */
		if(GM->EconComp)
		{
			GM->EconComp->SubtractGold(Army->GetPlayerID(), RecruitableUnits[Index]->GetGoldCostToCreate());
			GM->EconComp->SubtractFood(Army->GetPlayerID(), RecruitableUnits[Index]->GetFoodCostToCreate());
		}
		UpdatedArmyCards();
	}
}

void UArmyUserWidget::OnUnitCardArmyClicked(int Index)
{
	if(!Army || !GM || !GM->ArmyManager)
	{
		return;
	}

	if(Army->GetArmyComponent()->GetUnitDataArray().Num() == 1)
	{
		GM->ArmyManager->ArmyDestroyed(Army);
		GM->ArmyManager->OnArmiesUpdated.Broadcast();
		Controller->SetArmyUIVisibility(false);
	}
	else
	{
		Army->GetArmyComponent()->RemoveUnit(Army->GetArmyComponent()->GetUnitDataArray()[Index]);
		Controller->UpdateArmyUI(Army);
	}
}

void UArmyUserWidget::OnUnitCardArmyHovered(int Index)
{
	if(!Army)
	{
		return;
	}
	
	if(ArmyTooltip)
	{
		auto Unit = Army->GetArmyComponent()->GetUnitDataArray()[Index];
		if(Unit)
		{
			ArmyTooltip->UpdateTooltipUnit(Unit, true, true);
		}
	}
}

void UArmyUserWidget::OnUnitCardRecruitHovered(int Index)
{
	if(!Army)
	{
		return;
	}
	
	if(ArmyTooltip)
	{
		auto Unit = Army->GetRecruitableUnits()[Index];
		if(Unit)
		{
			ArmyTooltip->UpdateTooltipUnit(Unit, false, false);
		}
	}
}
