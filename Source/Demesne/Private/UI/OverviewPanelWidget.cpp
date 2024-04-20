// Created by WarRoom Wizards ©2023 - 2024


#include "UI/OverviewPanelWidget.h"

#include "DemesnePlayerController.h"
#include "StrategyLayerGameMode.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyManager.h"
#include "Military/ArmyUnit.h"
#include "Settlements/Settlement.h"
#include "Settlements/SettlementManager.h"
#include "UI/OverviewListWidget.h"

void UOverviewPanelWidget::RefreshSettlementUI()
{
	if(!GM || !GM->SettlementManager) return;

	/* Delete old widgets*/
	if(AllSettlementListWidgets.Num() > 0)
	{
		for (auto Widget : AllSettlementListWidgets)
		{
			Widget->RemoveFromParent();
		}

		AllSettlementListWidgets.Empty();
	}
	
	/* Delete old widgets*/
	if(SettlementListWidgets.Num() > 0)
	{
		for (auto Widget : SettlementListWidgets)
		{
			Widget->RemoveFromParent();
		}

		SettlementListWidgets.Empty();
	}
	
	
	TArray<ASettlement*> AllSettlements = GM->SettlementManager->GetSettlements();
	TArray<ASettlement*> PlayerSettlements = GM->SettlementManager->GetSettlementsByID(0);

	if(SettlementWidgetClass)
	{
		/* Create the ui for every settlement */
		for(const auto Settlement: AllSettlements)
		{
			UOverviewListWidget* Button = Cast<UOverviewListWidget>(CreateWidget(Controller, ArmyWidgetClass));

			if(Button && MainListSB)
			{
				MainListSB->AddChild(Button);
				AllSettlementListWidgets.Add(Button);

				if(Settlement)
				{
					Button->SetActor(Settlement);
					Button->SetName(Settlement->GetSettlementName());
				}
			}
		}

		/* Create the ui for player settlement */
		for(const auto Settlement : PlayerSettlements)
		{
			UOverviewListWidget* Button = Cast<UOverviewListWidget>(CreateWidget(Controller, ArmyWidgetClass));

			if(Button && SettlementListSB)
			{
				SettlementListSB->AddChild(Button);
				SettlementListWidgets.Add(Button);

				if(Settlement)
				{
					Button->SetActor(Settlement);
					Button->SetName(Settlement->GetSettlementName());
				}
			}
		}
	}
}

void UOverviewPanelWidget::RefreshArmyUI()
{
	if(!GM || !GM->ArmyManager) return;

	/* Delete old widgets*/
	if(ArmyListWidgets.Num() > 0)
	{
		for (auto Widget : ArmyListWidgets)
		{
			Widget->RemoveFromParent();
		}

		ArmyListWidgets.Empty();
	}
	
	TArray<AArmyUnit*> Armies = GM->ArmyManager->GetArmiesByID(0);

	if(ArmyWidgetClass)
	{
		for(auto Army : Armies)
		{
			UOverviewListWidget* Button = Cast<UOverviewListWidget>(CreateWidget(Controller, ArmyWidgetClass));

			if(Button && ArmyListSB)
			{
				ArmyListSB->AddChild(Button);
				ArmyListWidgets.Add(Button);

				Button->SetActor(Army);
				Button->SetName(Army->GetUnitName());
			}
		}
	}
}

void UOverviewPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GM = Cast<AStrategyLayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Controller = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if(!GM) return;

	GM->ArmyManager->OnArmiesUpdated.AddUniqueDynamic(this, &ThisClass::RefreshArmyUI);
	GM->SettlementManager->OnSettlementUpdated.AddUniqueDynamic(this, &ThisClass::RefreshSettlementUI);

	OpenCloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenClose);
	OverviewButton->OnClicked.AddUniqueDynamic(this, &ThisClass::SwitchWidgetAllSettlements);
	SettlementsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::SwitchWidgetSettlements);
	ArmiesButton->OnClicked.AddUniqueDynamic(this, &ThisClass::SwitchWidgetArmies);

	RefreshSettlementUI();
}

void UOverviewPanelWidget::SwitchWidgetAllSettlements()
{
	ListSwitcher->SetActiveWidget(MainListOverlay);
}

void UOverviewPanelWidget::SwitchWidgetSettlements()
{
	ListSwitcher->SetActiveWidget(SettlementsOverlay);
}

void UOverviewPanelWidget::SwitchWidgetArmies()
{
	ListSwitcher->SetActiveWidget(ArmiesOverlay);
}

void UOverviewPanelWidget::OpenClose()
{
	if(MainOverlay->GetVisibility() == ESlateVisibility::Visible)
	{
		MainOverlay->SetVisibility(ESlateVisibility::Collapsed);
		OpenCloseButtonIcon->SetRenderTransformAngle(180);
	}
	else
	{
		MainOverlay->SetVisibility(ESlateVisibility::Visible);
		OpenCloseButtonIcon->SetRenderTransformAngle(0);
	}
}
