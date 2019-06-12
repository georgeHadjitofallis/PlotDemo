// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphManager.h"
#include "Engine/StaticMeshActor.h"
#include "DataReader.h"
#include "Kismet/GameplayStatics.h"
#include "BarChart.h"
#include "DataItem.h"
#include "DataModel.h"

// Sets default values
AGraphManager::AGraphManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LoadBarChartBP();

	_graphsDefaultStartingPosition = FVector(-1800.0f, 1600.0f, -430.0f);

	_dataModel = MakeShared<DataModel>();

	LoadDefaultModel1D();
}


// Called when the game starts or when spawned
void AGraphManager::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateBarChart();
	
}

// Called every frame
void AGraphManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGraphManager::LoadBarChartBP()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/George/BarChart_BP.BarChart_BP'"));

	if (ItemBlueprint.Object) {
		_barchartBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

}

void AGraphManager::LoadDefaultModel1D()
{
	_dataModel->LoadData1D(FPaths::GameDir() + "demo.txt");
}

void AGraphManager::UpdateBarChart()
{
	ClearBarCharts();
	SpawnBarCharts();
}

void AGraphManager::ClearBarCharts()
{
	for (auto barchart : _barCharts)
	{
		barchart->Destroy();
	}
	_barCharts.Empty();
}

void AGraphManager::SpawnBarCharts()
{
	FVector currentSpawnPosition = _graphsDefaultStartingPosition;
	for (auto& item : _dataModel->getItem1D(_currentIndex).getData())
	{
		UE_LOG(LogTemp, Warning, TEXT(" Test %f"), item);

		FActorSpawnParameters SpawnParams;

		ABarChart* DroppedItem = GetWorld()->SpawnActor<ABarChart>(_barchartBlueprint, currentSpawnPosition, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
		TArray<UStaticMeshComponent*> Components;
		DroppedItem->GetComponents<UStaticMeshComponent>(Components);
		for (int32 i = 0; i < Components.Num(); i++)
		{
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
			StaticMeshComponent->SetRelativeScale3D(FVector(1, 1, FMath::Abs(item) *0.1));

			SetBarCharColor(StaticMeshComponent, item);

			FVector origin;
			FVector boxExtent;
			DroppedItem->GetActorBounds(false, origin, boxExtent);

			FVector ActorLocation = DroppedItem->GetActorLocation();
			if(item >0)
				ActorLocation.Z += boxExtent.Z;
			else 
				ActorLocation.Z -= boxExtent.Z;
			// Set the location- this will blindly place the actor at the given location  
			DroppedItem->SetActorLocation(ActorLocation, false);
		}

		
		DroppedItem->_barChartValue = item;
		_barCharts.Add(DroppedItem);
		currentSpawnPosition -= FVector(0, 500, 0);
	}
}

void AGraphManager::SetBarCharColor(UStaticMeshComponent* staticMeshComponent, float value)
{
	UMaterialInterface * Material = staticMeshComponent->GetMaterial(0);
	UMaterialInstanceDynamic* matInstance = staticMeshComponent->CreateDynamicMaterialInstance(0, Material);

	if (matInstance != nullptr)
	{
		if (value < 30)
		{
			matInstance->SetVectorParameterValue("Color", FVector(255, 0, 0));
		}
		else if(value<70)
			matInstance->SetVectorParameterValue("Color", FVector(255, 255, 0)); 
		else
		{
			matInstance->SetVectorParameterValue("Color", FVector(0, 255, 0));
		}
	}
}

int AGraphManager::GetNumberOfItems1D()
{
	return _dataModel->GetNumberOfItems();
}

void AGraphManager::SetIndex(int index)
{
	_currentIndex = index;
	UpdateBarChart();
}

void AGraphManager::IncreaseIndex()
{
	if (_currentIndex < _dataModel->GetNumberOfItems() - 1)
	{
		_currentIndex++;
		UpdateBarChart();
	}
}

void AGraphManager::DecreaseIndex()
{
	if (_currentIndex >0)
	{
		_currentIndex--;
		UpdateBarChart();
	}
}