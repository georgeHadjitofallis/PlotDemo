// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"
#include "Net/OnlineEngineInterface.h"
#include "PlotDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PLOTDEMO_API APlotDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
