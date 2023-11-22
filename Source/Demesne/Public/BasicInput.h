// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BasicInput.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBasicInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEMESNE_API IBasicInput
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void Move(const FVector2D& MovementVector);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void Look(const FVector2D& LookVector);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void Zoom(const float& LookVector);
};
