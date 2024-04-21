// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Military/UnitData.h"
#include "UnitCardWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotButtonDelegate, int, Index);

UCLASS()
class DEMESNE_API UUnitCardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	FOnSlotButtonDelegate OnHoveredDelegate;
	
	UFUNCTION()
	void SetUnitImage(UTexture2D* Image) const { UnitImage->SetBrushFromTexture(Image); }

	UFUNCTION()
	void SetIndex(const int InIndex) { Index = InIndex;}

	UFUNCTION()
	void UpdateUnitCard(UUnitData* UnitData);
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UImage* UnitImage;

	UPROPERTY(meta=(BindWidget))
	class UButton* UnitCardButton;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY()
	int Index;
	
	UFUNCTION()
	virtual void OnButtonClicked();
	
	UFUNCTION()
	virtual void OnButtonHovered();
};
