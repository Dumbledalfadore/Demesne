// Created by WarRoom Wizards ©2023


#include "UI/ArmyUserWidget.h"
#include "DemesnePlayerController.h"
#include "EconomyComponent.h"
#include "StrategyLayerGameMode.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyDataComponent.h"
#include "Military/ArmyUnit.h"
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
				UUnitCardButtonWidget* UnitCardWidget = CreateWidget<UUnitCardButtonWidget>(Controller, UnitCardButtonWidgetClass);
				if(UnitCardWidget)
				{
					UnitCardWidget->SetIndex(Index);
					UnitCardButtonWidgets.Add(UnitCardWidget);
					ArmyWrapBox->AddChild(UnitCardWidget);
					//UE_LOG(LogTemp, Warning, TEXT("ARMY UPDATED"));
					UnitCardWidget->UpdateUnitCard(Unit);
				}
			}
			Index++;
		}
	}
}

void UArmyUserWidget::ShowRecruits()
{
	UE_LOG(LogTemp, Warning, TEXT("SHOW RECRUITS"));
	ESlateVisibility Vis = RecruitArmyOverlay->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
	RecruitArmyOverlay->SetVisibility(Vis);

	/* Destroy old cards */
	if(RecruitUnitCardButtonWidgets.Num() > 0)
	{
		for(auto Card : RecruitUnitCardButtonWidgets)
		{
			Card->OnClickedDelegate.RemoveDynamic(this, &ThisClass::RecruitUnit);
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
				UnitCardButtonWidget->SetIndex(Index);
				//UE_LOG(LogTemp, Warning, TEXT("ARMY UPDATED"));
				UnitCardButtonWidget->UpdateUnitCard(Unit);
				UnitCardButtonWidget->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::RecruitUnit);
			}
		}
		Index++;
	}
}

void UArmyUserWidget::RecruitUnit(int Index)
{
	auto RecruitableUnits = Army->GetRecruitableUnits();
	if(RecruitableUnits[Index])
	{
		Army->GetArmyComponent()->AddUnit(RecruitableUnits[Index]);
		UpdatedArmyCards();
	}
}
