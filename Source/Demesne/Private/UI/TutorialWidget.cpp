// Created by WarRoom Wizards ©2023 - 2024


#include "UI/TutorialWidget.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "HAL/FileManager.h"
void UTutorialWidget::ReadInTasks()
{
	const char wordSpace = ' ';
	const char newLine = '\n';
	char characters;
	FString tempTaskText;
	FTaskListFormat tempTask;
	int taskNum = 1;
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*(FPaths::ProjectContentDir() + "/" + FilePath))) return;
	
	std::ifstream inputFile(*(FPaths::ProjectContentDir() + "/" + FilePath));

	inputFile >> std::noskipws;
	if (inputFile.is_open())
	{
		while (inputFile)
		{
			inputFile >> characters;
			if (characters == newLine || inputFile.eof())
			{
				tempTask.TaskText = tempTaskText;
				tempTask.TaskNum = taskNum;


				TaskList.Add(tempTask);
				taskNum++;
				tempTaskText = {};
			}
			else
			{
				tempTaskText += characters;
			}
		}
	}
}

void UTutorialWidget::TaskCompleted(int taskNum)
{

	for (int i = 0; i < TaskList.Num(); i++)
	{

		if (taskNum == TaskList[i].TaskNum)
		{
			CompletedTaskList.Add(TaskList[i]);
			TaskList.RemoveAt(i);
		}
	}
}

void UTutorialWidget::UpdateVisibleTasks()
{
	VisibleTasks.Empty();
	int availableTasks = TaskList.Num() - 1;
	if (availableTasks < 0) availableTasks = 0;
	for (int i = 0; i < VisTaskQuant; i++)
	{
		if (i > availableTasks)
		{
			VisibleTasks.Add("");
		}
		else
		{
			VisibleTasks.Add(TaskList[i].TaskText);
		}
	}
}

FString UTutorialWidget::GetTasks(int TaskNum)
{
	return VisibleTasks[TaskNum];
}

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OpenCloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenClose);
	
}

void UTutorialWidget::OpenClose()
{
	if (MainOverlay->GetVisibility() == ESlateVisibility::Visible)
	{
		MainOverlay->SetVisibility(ESlateVisibility::Collapsed);
		OpenCloseButtonIcon->SetRenderTransformAngle(180);
	}
	else
	{
		MainOverlay->SetVisibility(ESlateVisibility::Visible);
		OpenCloseButtonIcon->SetRenderTransformAngle(0);

		if (!Task6Complete)
		{
			TaskCompleted(6);
			Task6Complete = true;
		}
	}
}
