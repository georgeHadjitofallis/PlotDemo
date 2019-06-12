// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class Item;

class PLOTDEMO_API DataReader
{
public:
	
	static TArray<Item> LoadFile1D(FString filePath);
	static TArray<FVector> LoadFile3D(FString filePath);
private:
	static TArray<FString> LoadFileInternal(FString filePath);
	static TArray<Item> ParseLinesToInt(const TArray<FString>& lines);
	static TArray<FVector> ParseLinesToVector(const TArray<FString>& lines);
};
