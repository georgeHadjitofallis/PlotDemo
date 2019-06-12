// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class Item;
/**
 * 
 */
class PLOTDEMO_API DataModel
{
public:
	DataModel();
	~DataModel();

	void LoadData1D(FString pathToFile);
	Item getItem1D(int index);
	int GetNumberOfItems();
private:
	TArray<Item> _data;
};


class PLOTDEMO_API DataModel3D
{
public:
	DataModel3D();
	~DataModel3D();

	void LoadData3D(FString pathToFile);
	FVector getItem3D(int index);
	int GetNumberOfItems();
	TArray<FVector> getData();
private:
	TArray<FVector> _data;
};