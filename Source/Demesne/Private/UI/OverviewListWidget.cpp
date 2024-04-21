// Created by WarRoom Wizards ©2023 - 2024


#include "UI/OverviewListWidget.h"

#include "DemesnePlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Settlements/Settlement.h"
#include "UI/OverviewPanelWidget.h"

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

		ADemesnePlayerController* PC = Cast<ADemesnePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if(PC)
		{
			if(ASettlement* Settlement = Cast<ASettlement>(BoundActor))
			{
				PC->UpdateSettlementUI(Settlement);
				PC->SetArmyUIVisibility(false);
			}
			else if(AArmyUnit* Army = Cast<AArmyUnit>(BoundActor))
			{
				PC->UpdateArmyUI(Army);
				PC->SetSettlementUIVisibility(false);
			}
		}

	}
}
