// Created by WarRoom Wizards ©2023 - 2024

#pragma once

#include "CoreMinimal.h"
#include "TDCameraPawn.h"
#include "Blueprint/UserWidget.h"
#include "OverviewListWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API UOverviewListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetName(const FString& Name) const;

	UFUNCTION()
	void SetActor(AActor* Actor) { BoundActor = Actor; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnButtonClicked();
	
	UPROPERTY()
	AActor* BoundActor;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(meta=(BindWidget))
	class UButton* ListingButton;

	UPROPERTY()
	ATDCameraPawn* Camera;
	
};
