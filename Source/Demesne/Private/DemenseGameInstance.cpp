// Fill out your copyright notice in the Description page of Project Settings.


#include "DemenseGameInstance.h"

#include "MoviePlayer.h"

void UDemenseGameInstance::Init()
{
	Super::Init();
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDemenseGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDemenseGameInstance::EndLoadingScreen);
}

void UDemenseGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 0.5f;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
 
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UDemenseGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
}
