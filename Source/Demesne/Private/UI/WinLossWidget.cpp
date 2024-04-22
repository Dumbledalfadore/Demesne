// Created by WarRoom Wizards ©2023 - 2024

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/WinLossWidget.h"
#include <Kismet/GameplayStatics.h>

void UWinLossWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EndButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ButtonPressed);
}

void UWinLossWidget::WinState(bool State)
{
	if (State)
	{
		WinText->SetVisibility(ESlateVisibility::Visible);
		LossText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		WinText->SetVisibility(ESlateVisibility::Hidden);
		LossText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWinLossWidget::ButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}
