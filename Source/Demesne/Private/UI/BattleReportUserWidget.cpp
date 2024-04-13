// Created by WarRoom Wizards ©2023


#include "UI/BattleReportUserWidget.h"

#include "DemesnePlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "Military/ArmyDataComponent.h"
#include "UI/UnitCardWidget.h"


void UBattleReportUserWidget::OnBattleReported(UArmyDataComponent* Aggressor, UArmyDataComponent* Defender, FString Victor)
{
	/* Delete old cards */
	for(auto Card : UnitCardWidgets)
	{
		Card->RemoveFromParent();
	}
	
	if(Victor == "Aggressor")
	{
		AggressorVictoryBackground->SetVisibility(ESlateVisibility::Visible);
		AggressorDefeatBackground->SetVisibility(ESlateVisibility::Collapsed);
		AggressorStalemateBackground->SetVisibility(ESlateVisibility::Collapsed);

		DefenderVictoryBackground->SetVisibility(ESlateVisibility::Collapsed);
		DefenderDefeatBackground->SetVisibility(ESlateVisibility::Visible);
		DefenderStalemateBackground->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if(Victor == "Defender")
	{
		AggressorVictoryBackground->SetVisibility(ESlateVisibility::Collapsed);
		AggressorDefeatBackground->SetVisibility(ESlateVisibility::Visible);
		AggressorStalemateBackground->SetVisibility(ESlateVisibility::Collapsed);

		DefenderVictoryBackground->SetVisibility(ESlateVisibility::Visible);
		DefenderDefeatBackground->SetVisibility(ESlateVisibility::Collapsed);
		DefenderStalemateBackground->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		AggressorVictoryBackground->SetVisibility(ESlateVisibility::Collapsed);
		AggressorDefeatBackground->SetVisibility(ESlateVisibility::Collapsed);
		AggressorStalemateBackground->SetVisibility(ESlateVisibility::Visible);

		DefenderVictoryBackground->SetVisibility(ESlateVisibility::Collapsed);
		DefenderDefeatBackground->SetVisibility(ESlateVisibility::Collapsed);
		DefenderStalemateBackground->SetVisibility(ESlateVisibility::Visible);
	}

	FString PowerPrefix = "Power: ";

	float AggressorPower = 	FMath::RoundHalfToZero(Aggressor->CalculatePower());
	float DefenderPower = FMath::RoundHalfToZero(Defender->CalculatePower());
	
	AggressorPowerText->SetText(FText::FromString(PowerPrefix + FString::SanitizeFloat(AggressorPower, 0)));
	DefenderPowerText->SetText(FText::FromString(PowerPrefix + FString::SanitizeFloat(DefenderPower, 0)));

	/* Create new cards */
	if(UnitCardWidgetClass)
	{
		/* Aggressor */
		for(auto Unit : Aggressor->GetUnitDataArray())
		{
			UUnitCardWidget* UnitCardWidget = CreateWidget<UUnitCardWidget>(Controller, UnitCardWidgetClass);
			AggressorWrapBox->AddChild(UnitCardWidget);
			UnitCardWidgets.Add(UnitCardWidget);
			UnitCardWidget->UpdateUnitCard(Unit);
		}

		/* Defender */
		for(auto Unit : Defender->GetUnitDataArray())
		{
			UUnitCardWidget* UnitCardWidget = CreateWidget<UUnitCardWidget>(Controller, UnitCardWidgetClass);
			DefenderWrapBox->AddChild(UnitCardWidget);
			UnitCardWidgets.Add(UnitCardWidget);
			UnitCardWidget->UpdateUnitCard(Unit);
		}
	}
}

void UBattleReportUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Controller = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
