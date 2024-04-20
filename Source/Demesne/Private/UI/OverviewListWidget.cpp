// Created by WarRoom Wizards ©2023 - 2024


#include "UI/OverviewListWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UOverviewListWidget::SetName(const FString& Name) const
{
	if(!NameText) return;
	
	NameText->SetText(FText::FromString(Name));
}

void UOverviewListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ListingButton)
	{
		ListingButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
	}

	Camera = Cast<ATDCameraPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UOverviewListWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(ListingButton)
	{
		ListingButton->OnClicked.RemoveDynamic(this, &ThisClass::OnButtonClicked);
	}
}

void UOverviewListWidget::OnButtonClicked()
{
	if(Camera)
	{
		Camera->SetLocation(BoundActor->GetActorLocation());
	}
}
