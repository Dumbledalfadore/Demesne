// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UnitCardWidget.generated.h"


UCLASS()
class DEMESNE_API UUnitCardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetUnitImage(UTexture2D* Image) const { UnitImage->SetBrushFromTexture(Image); }

	UFUNCTION()
	void SetIndex(const int InIndex) { Index = InIndex;}
	
protected:
	UPROPERTY()
	int Index;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* UnitImage;
};
