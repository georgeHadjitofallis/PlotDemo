// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphManager.generated.h"

class ABarChart;
class APlotPoint;
class DataModel;
class DataModel3D;

UCLASS()
class PLOTDEMO_API AGraphManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraphManager();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadBarChartBP();

	void LoadPlotPointBP();

	void LoadDefaultModels();

	void UpdateBarChart();

	void UpdatePlot();

	void ClearPlotPoits();

	UFUNCTION(BlueprintCallable)
	int GetNumberOfItems1D();

	UFUNCTION(BlueprintCallable)
	void SetIndex(int index);

	UFUNCTION(BlueprintCallable)
	void IncreaseIndex();

	UFUNCTION(BlueprintCallable)
	void DecreaseIndex();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABarChart> _barchartBlueprint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlotPoint> _plotPointBlueprint;
private:

	void ClearBarCharts();

	void SpawnPlotPoints();

	void SpawnBarCharts();

	void SetBarCharColor(UStaticMeshComponent * staticMeshComponent, float value);

	TSharedPtr<DataModel> _dataModel;
	TSharedPtr<DataModel3D> _dataModel3D;
	TArray<ABarChart*> _barCharts;
	TArray<APlotPoint*> _plotPoints;
	FVector _graphsDefaultStartingPosition;
	int _currentIndex = 0;
};
