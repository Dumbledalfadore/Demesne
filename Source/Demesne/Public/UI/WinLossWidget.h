// Created by WarRoom Wizards ©2023 - 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinLossWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMESNE_API UWinLossWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LossText;
	UPROPERTY(meta = (BindWidget))
	class UButton* EndButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void WinState(bool State);

	UFUNCTION()
	void ButtonPressed();

protected:



};
