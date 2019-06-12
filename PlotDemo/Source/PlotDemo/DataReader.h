// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class Item;

class PLOTDEMO_API DataReader
{
public:
	
	static TArray<Item> LoadFile(FString filePath);

private:
	static TArray<Item> LoadFileInternal(FString filePath);
	static TArray<Item> ParseLines(const TArray<FString>& lines);
};
