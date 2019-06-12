// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphManager.generated.h"

class ABarChart;
class DataModel;
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

	void LoadDefaultModel1D();

	void UpdateBarChart();

	

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
private:

	void ClearBarCharts();

	void SpawnBarCharts();

	void SetBarCharColor(UStaticMeshComponent * staticMeshComponent, float value);

	TSharedPtr<DataModel> _dataModel;
	TArray<ABarChart*> _barCharts;
	
	FVector _graphsDefaultStartingPosition;
	int _currentIndex = 0;
};
