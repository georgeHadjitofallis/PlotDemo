// Fill out your copyright notice in the Description page of Project Settings.


#include "DataModel.h"
#include "DataReader.h"
#include "DataItem.h"

// Data Model 1d
DataModel::DataModel()
{
}

DataModel::~DataModel()
{
}

void DataModel::LoadData1D(FString pathToFile)
{
	_data = DataReader::LoadFile1D(pathToFile);
}

Item DataModel::getItem1D(int index)
{
	if (index >= 0 && index < _data.Num())
		return _data[index];
	return Item();
}

int DataModel::GetNumberOfItems()
{
	return _data.Num();
}

// Data Model 3d
DataModel3D::DataModel3D()
{
}

DataModel3D::~DataModel3D()
{
}

void DataModel3D::LoadData3D(FString pathToFile)
{
	_data = DataReader::LoadFile3D(pathToFile);
}

FVector DataModel3D::getItem3D(int index)
{
	if (index >= 0 && index < _data.Num())
		return _data[index];
	return FVector();
}

int DataModel3D::GetNumberOfItems()
{
	return _data.Num();
}

TArray<FVector> DataModel3D::getData()
{
	return _data;
}
