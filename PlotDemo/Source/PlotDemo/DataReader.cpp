// Fill out your copyright notice in the Description page of Project Settings.


#include "DataReader.h"
#include "PlatformFilemanager.h"
#include "DataItem.h"

TArray<Item> DataReader::LoadFile1D(FString filePath)
{

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Not Find File"));
		return TArray<Item>();
	}
	TArray<FString> lines=  DataReader::LoadFileInternal(filePath);
	return DataReader::ParseLinesToInt(lines);
}

TArray<FVector> DataReader::LoadFile3D(FString filePath)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Not Find File"));
		return TArray<FVector>();
	}
	TArray<FString> lines = DataReader::LoadFileInternal(filePath);
	return DataReader::ParseLinesToVector(lines);
}

TArray<FString> DataReader::LoadFileInternal(FString filePath)
{
	FString fileContent = "";

	FFileHelper::LoadFileToString(fileContent, *filePath);

	TArray<FString> lines;
	fileContent.ParseIntoArray(lines, TEXT("\n"), true);
	return lines;
	
}

TArray<Item> DataReader::ParseLinesToInt(const TArray<FString>& lines)
{
	TArray<Item> lineData;
	for (auto line : lines)
	{
		TArray<FString> currentlineData;
		line.ParseIntoArray(currentlineData, TEXT(","), true);
		Item item;
		for (int index = 0; index != currentlineData.Num(); ++index)
		{
			float MyShinyNewInt = FCString::Atof(*currentlineData[index]);
			item.AddData(MyShinyNewInt);
		}
		lineData.Add(item);

	}
	return lineData;
}

TArray<FVector> DataReader::ParseLinesToVector(const TArray<FString>& lines)
{
	TArray<FVector> lineData;
	for (auto line : lines)
	{
		TArray<FString> currentlineData;
		line.ParseIntoArray(currentlineData, TEXT(","), true);
		TArray<FVector> item;
		if (currentlineData.Num() != 3)
			throw "Invalid number of Items";
		FVector data = FVector(FCString::Atof(*currentlineData[0]), FCString::Atof(*currentlineData[1]), FCString::Atof(*currentlineData[2]));
		lineData.Add(data);

	}
	return lineData;
}