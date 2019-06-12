// Fill out your copyright notice in the Description page of Project Settings.


#include "DataModel.h"
#include "DataReader.h"
#include "DataItem.h"

DataModel::DataModel()
{
}

DataModel::~DataModel()
{
}

void DataModel::LoadData1D(FString pathToFile)
{
	_data = DataReader::LoadFile(pathToFile);
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
