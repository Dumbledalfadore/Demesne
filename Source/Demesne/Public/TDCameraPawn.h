// Created by WarRoom Wizards ©2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasicInput.h"
#include "TDCameraPawn.generated.h"

UCLASS()
class DEMESNE_API ATDCameraPawn : public APawn, public IBasicInput
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATDCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Input Implementations */
	virtual void Move_Implementation(const FVector2D& MovementVector) override;

	virtual void Look_Implementation(const FVector2D& LookVector) override;

	virtual void Zoom_Implementation(const float& LookVector) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FRotator DefaultRotation = FRotator(-70.f, 0.f, 0.f);

	/* Most we can zoom out */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float SpringArmMax = 10000.f; //Inflated whilst using standard map due to vast scale of it.

	/* Most we can zoom in */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float SpringArmMin = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float MovementSpeed = 10000.f; //Inflated whilst using standard map due to vast scale of it.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float ZoomSpeed = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float RotateSpeed = 35.f;

	UPROPERTY()
	class UFloatingPawnMovement* Movement;
};
