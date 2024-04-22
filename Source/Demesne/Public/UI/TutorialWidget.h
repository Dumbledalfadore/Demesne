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

	// Reads in the txt file 
	UFUNCTION()
	void ReadInTasks();

	//removes a task from the array
	UFUNCTION()
	void TaskCompleted(int taskNum);
	//on each tick of the player controller the tasks are sorted by num lowest come first
	UFUNCTION()
	void UpdateVisibleTasks();

	//gets one of the 6 visible tasks
	UFUNCTION(BlueprintPure)
	FString GetTasks(int TaskNum);

	//Widget components 
	UPROPERTY(meta = (BindWidget))
	class UOverlay* MainOverlay;
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenCloseButton;
	UPROPERTY(meta = (BindWidget))
	class UImage* OpenCloseButtonIcon;
protected:
	virtual void NativeConstruct() override;
	/*UPROPERTY()
	class ADemesnePlayerController* PlayerControllerRef;*/
	UFUNCTION()
	void OpenClose();

private:

	//array fot task list
	UPROPERTY(VisibleAnywhere)
	TArray<FTaskListFormat> TaskList;

	//array for the completed tasks 
	UPROPERTY(VisibleAnywhere)
	TArray<FTaskListFormat> CompletedTaskList;

	//file path address
	FString FilePath = "TaskList.txt";
	UPROPERTY(EditAnywhere)
	int VisTaskQuant = 6;
	UPROPERTY(VisibleAnywhere)
	TArray<FString> VisibleTasks;
	//std::string FileAddress = "Demesne\TaskList.txt";
};
