// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphManager.h"
#include "Engine/StaticMeshActor.h"
#include "DataReader.h"
#include "Kismet/GameplayStatics.h"
#include "BarChart.h"
#include "DataItem.h"
#include "PlotPoint.h"
#include "DataModel.h"
#include "DrawDebugHelpers.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"

// Sets default values
AGraphManager::AGraphManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LoadBarChartBP();
	LoadPlotPointBP();

	_graphsDefaultStartingPosition = FVector(-1800.0f, 1600.0f, -430.0f);

	_dataModel = MakeShared<DataModel>();
	_dataModel3D = MakeShared<DataModel3D>();

	LoadDefaultModels();
}


// Called when the game starts or when spawned
void AGraphManager::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateBarChart();
	UpdatePlot();
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

void AGraphManager::LoadPlotPointBP()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/George/PlotPoint_BP.PlotPoint_BP'"));

	if (ItemBlueprint.Object) {
		_plotPointBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

}

void AGraphManager::LoadDefaultModels()
{
	_dataModel->LoadData1D(FPaths::GameDir() + "DataFiles/demo.txt");
	_dataModel3D->LoadData3D(FPaths::GameDir() + "DataFiles/Data3D.txt");
}

void AGraphManager::UpdateBarChart()
{
	ClearBarCharts();
	SpawnBarCharts();
}

void AGraphManager::UpdatePlot()
{
	ClearPlotPoits();
	FlushPersistentDebugLines(GetWorld());
	SpawnPlotPoints();
}

void AGraphManager::ClearPlotPoits()
{
	for (auto plotPoint : _plotPoints)
	{
		plotPoint->Destroy();
	}
	_plotPoints.Empty();
}

void AGraphManager::ClearBarCharts()
{
	for (auto barchart : _barCharts)
	{
		barchart->Destroy();
	}
	_barCharts.Empty();
}

void AGraphManager::SpawnPlotPoints()
{
	for (auto& item : _dataModel3D->getData())
	{
		FActorSpawnParameters SpawnParams;

		APlotPoint* DroppedItem = GetWorld()->SpawnActor<APlotPoint>(_plotPointBlueprint, item, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
		TArray<UStaticMeshComponent*> Components;
		DroppedItem->GetComponents<UStaticMeshComponent>(Components);

		_plotPoints.Add(DroppedItem);
	}

	FVector origin;
	FVector boxExtent;
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _plotPointBlueprint, actors);
	UGameplayStatics::GetActorArrayBounds(actors, false, origin, boxExtent);
	DrawDebugBox(
		GetWorld(),
		origin,
		boxExtent,
		FColor(255, 0, 0), 
		true, 
		30, 
		5
	);
}

void AGraphManager::SpawnBarCharts()
{
	FVector currentSpawnPosition = _graphsDefaultStartingPosition;
	for (auto& item : _dataModel->getItem1D(_currentIndex).getData())
	{
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

void AGraphManager::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames)
{
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				uint32 SelectionFlag = 0; 
				DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
			}
		}
	}
}

void AGraphManager::LoadBarChartFile(const FString& File)
{
	_dataModel->LoadData1D(File);
	_currentIndex = 0;
	UpdateBarChart();
}

void AGraphManager::LoadPLotFile(const FString& File)
{
	_dataModel3D->LoadData3D(File);
	UpdatePlot();
}