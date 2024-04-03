// Created by WarRoom Wizards ©2023


#include "UI/SettlementButtonUserWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

void USettlementButtonUserWidget::SetIcon(UTexture2D* NewIcon)
{
	BuildingIcon->SetBrushFromTexture(NewIcon);
}

void USettlementButtonUserWidget::SetBackground(FLinearColor Colour)
{
	SlotButton->SetBackgroundColor(Colour);
}

void USettlementButtonUserWidget::SetTier(FString Tier)
{
	BuildingTierText->SetText(FText::FromString(Tier));
}

void USettlementButtonUserWidget::SetPopulation(FString Population)
{
	PopulationText->SetText(FText::FromString(Population));
}

void USettlementButtonUserWidget::SetPopulationVisible(ESlateVisibility Visible)
{
	PopulationOverlay->SetVisibility(Visible);
}

void USettlementButtonUserWidget::SetIsLocked(bool bIsLocked)
{
	LockedIcon->SetVisibility(bIsLocked ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	BuildingIcon->SetVisibility(!bIsLocked ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void USettlementButtonUserWidget::SetBorderType(FString BorderType)
{
	if(BorderType == "Upgrade")
	{
		//UE_LOG(LogTemp, Warning, TEXT("BT:UPGRADES"));
		UpgradeBorder->SetVisibility(ESlateVisibility::Visible);
		NewBorder->SetVisibility(ESlateVisibility::Collapsed);
		DestroyBorder->SetVisibility(ESlateVisibility::Collapsed);
		StandardBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if(BorderType == "New")
	{
		//UE_LOG(LogTemp, Warning, TEXT("BT:NEW"));
		UpgradeBorder->SetVisibility(ESlateVisibility::Collapsed);
		NewBorder->SetVisibility(ESlateVisibility::Visible);
		DestroyBorder->SetVisibility(ESlateVisibility::Collapsed);
		StandardBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if(BorderType == "Destroy")
	{
		//UE_LOG(LogTemp, Warning, TEXT("BT:DESTROY"));
		UpgradeBorder->SetVisibility(ESlateVisibility::Collapsed);
		NewBorder->SetVisibility(ESlateVisibility::Collapsed);
		DestroyBorder->SetVisibility(ESlateVisibility::Visible);
		StandardBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("BT:STANDARD"));
		UpgradeBorder->SetVisibility(ESlateVisibility::Collapsed);
		NewBorder->SetVisibility(ESlateVisibility::Collapsed);
		DestroyBorder->SetVisibility(ESlateVisibility::Collapsed);
		StandardBorder->SetVisibility(ESlateVisibility::Visible);
	}
}

void USettlementButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SlotButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
	SlotButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnButtonHovered);
	HiddenButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnButtonHovered); /* Done to fix an issue with the tooltip not showing on disabled slots */
}

void USettlementButtonUserWidget::NativeDestruct()
{
	SlotButton->OnClicked.RemoveDynamic(this, &ThisClass::OnButtonClicked);
	SlotButton->OnHovered.RemoveDynamic(this, &ThisClass::OnButtonHovered);
	HiddenButton->OnHovered.RemoveDynamic(this, &ThisClass::OnButtonHovered);
}

void USettlementButtonUserWidget::OnButtonClicked()
{
	OnClickedDelegate.Broadcast(Index);
}

void USettlementButtonUserWidget::OnButtonHovered()
{
	OnHoveredDelegate.Broadcast(Index);
}
