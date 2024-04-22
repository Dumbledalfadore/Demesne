// Created by WarRoom Wizards ©2023 - 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <fstream>
#include "TutorialWidget.generated.h"

/**
 * 
 */
USTRUCT()
struct FTaskListFormat
{
	GENERATED_USTRUCT_BODY()
	int TaskNum;
	FString TaskText;
	bool completed = false;
};

UCLASS()
class DEMESNE_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void ReadInTasks();
	UFUNCTION()
	void TaskCompleted(int taskNum);
	UFUNCTION()
	void UpdateVisibleTasks();
	UFUNCTION(BlueprintPure)
	FString GetTasks(int TaskNum);


	UPROPERTY(meta = (BindWidget))
	class UOverlay* MainOverlay;

protected:


private:
	UPROPERTY(VisibleAnywhere)
	TArray<FTaskListFormat> TaskList;
	UPROPERTY(VisibleAnywhere)
	TArray<FTaskListFormat> CompletedTaskList;
	FString FilePath = "TaskList.txt";
	UPROPERTY(EditAnywhere)
	int VisTaskQuant = 6;
	UPROPERTY(VisibleAnywhere)
	TArray<FString> VisibleTasks;
	//std::string FileAddress = "Demesne\TaskList.txt";
};
