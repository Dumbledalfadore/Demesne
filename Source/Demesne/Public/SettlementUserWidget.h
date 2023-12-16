// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettlementUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API USettlementUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Buttons")
	class UButton* BuildingSlotButton;

	UPROPERTY(BlueprintReadOnly)
	class AActor* Owner;
};
