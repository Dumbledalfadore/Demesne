// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleReportUserWidget.generated.h"

class UArmyDataComponent;
class AArmyUnit;
/**
 * 
 */
UCLASS()
class DEMESNE_API UBattleReportUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Updates the UI */
	UFUNCTION()
	void OnBattleReported(UArmyDataComponent* Aggressor, UArmyDataComponent* Defender, FString Victor);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* AggressorVictoryBackground;

	UPROPERTY(meta=(BindWidget))
	class UImage* AggressorDefeatBackground;

	UPROPERTY(meta=(BindWidget))
	class UImage* AggressorStalemateBackground;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* DefenderVictoryBackground;

	UPROPERTY(meta=(BindWidget))
	class UImage* DefenderDefeatBackground;

	UPROPERTY(meta=(BindWidget))
	class UImage* DefenderStalemateBackground;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AggressorPowerText;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DefenderPowerText;
	
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* AggressorWrapBox;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* DefenderWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards", meta=(ExposeOnSpawn))
	TSubclassOf<class UUnitCardWidget> UnitCardWidgetClass;

	UPROPERTY()
	TArray<class UUnitCardWidget*> UnitCardWidgets;

	UPROPERTY()
	class ADemesnePlayerController* Controller;
};
