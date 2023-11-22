// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "DemesnePlayerController.generated.h"

/**
 * 
 */
class UInputAction;

UCLASS()
class DEMESNE_API ADemesnePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/* Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomInput;

protected:
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
};
