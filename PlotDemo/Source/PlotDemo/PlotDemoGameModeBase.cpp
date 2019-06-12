// Fill out your copyright notice in the Description page of Project Settings.


#include "PlotDemoGameModeBase.h"
#include "PlotDemoPlayerController.h"

void APlotDemoGameModeBase::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	UWorld* World = GetWorld();

	// Save Options for future use
	OptionsString = Options;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save game sessions into a map
	GameSession = World->SpawnActor<AGameSession>(GetGameSessionClass(), SpawnInfo);
	GameSession->InitOptions(Options);

	FGameModeEvents::GameModeInitializedEvent.Broadcast(this);
	if (GetNetMode() != NM_Standalone)
	{
		// Attempt to login, returning true means an async login is in flight
		if (!UOnlineEngineInterface::Get()->DoesSessionExist(World, GameSession->SessionName) &&
			!GameSession->ProcessAutoLogin())
		{
			GameSession->RegisterServer();
		}
	}
}
