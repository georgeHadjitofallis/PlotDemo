// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class  PLOTDEMO_API Item
{
private:
	TArray<float> _data;
public:
	Item();
	
	void AddData(float inputData);
	
	int Count();
	TArray<float> getData();
};
