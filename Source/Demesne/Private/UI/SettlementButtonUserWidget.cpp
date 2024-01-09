// Created by WarRoom Wizards ©2023


#include "UI/SettlementButtonUserWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
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

void USettlementButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SlotButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
}

void USettlementButtonUserWidget::NativeDestruct()
{
	SlotButton->OnClicked.RemoveDynamic(this, &ThisClass::OnButtonClicked);
}

void USettlementButtonUserWidget::OnButtonClicked()
{
	OnClickedDelegate.Broadcast(Index);
}
