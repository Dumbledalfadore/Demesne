// Created by WarRoom Wizards ©2023 - 2024


#include "Utils/FileParser.h"

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}

bool FileParser::LoadFileToStringArray(FString FileName, TArray<FString>& StringArray)
{
	UE_LOG(LogTemp, Warning, TEXT("LOADING: %s"), *FPaths::ProjectContentDir());
	return FFileHelper::LoadFileToStringArray(StringArray, *(FPaths::ProjectContentDir() + "/" + FileName));
}
