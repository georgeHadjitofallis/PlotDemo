// Fill out your copyright notice in the Description page of Project Settings.


#include "DataReader.h"
#include "PlatformFilemanager.h"
#include "DataItem.h"

TArray<Item> DataReader::LoadFile(FString filePath)
{

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Not Find File"));
		return TArray<Item>();
	}
	return DataReader::LoadFileInternal(filePath);

}

TArray<Item> DataReader::LoadFileInternal(FString filePath)
{
	FString fileContent = "";

	FFileHelper::LoadFileToString(fileContent, *filePath);

	TArray<FString> lines;
	fileContent.ParseIntoArray(lines, TEXT("\n"), true);

	return DataReader::ParseLines(lines);
}

TArray<Item> DataReader::ParseLines(const TArray<FString>& lines)
{
	TArray<float> result;
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
			result.Add(MyShinyNewInt);
		}
		lineData.Add(item);

	}
	return lineData;
}
