// Created by WarRoom Wizards ©2023 - 2024

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEMESNE_API FileParser
{
public:
	FileParser();
	~FileParser();
	
	static bool LoadFileToStringArray(FString FileName, TArray<FString>& StringArray);
};
