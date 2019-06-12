// Fill out your copyright notice in the Description page of Project Settings.


#include "DataItem.h"


Item::Item()
{
}

void Item::AddData(float inputData)
{
	
	_data.Add(inputData);
	
}

int Item::Count()
{
	return _data.Num();
}

TArray<float> Item::getData()
{
	return _data;
}
