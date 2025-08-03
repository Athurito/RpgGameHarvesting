// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/HarvestActionMappingConfig.h"

UHarvestableConfig* UHarvestActionMappingConfig::GetDataForTag(FGameplayTag Tag) const
{
	if (const TSoftObjectPtr<UHarvestableConfig>* AssetPtr = TagMap.Find(Tag))
	{
		// Synchronously load the asset if needed
		return AssetPtr->LoadSynchronous();
	}
	return nullptr;
}
