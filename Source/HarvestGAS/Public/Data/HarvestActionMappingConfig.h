// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HarvestableConfig.h"
#include "Engine/DataAsset.h"
#include "HarvestActionMappingConfig.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta=(DisplayName="Harvest Action Mapping Config", PrimaryAssetType="HarvestActionMappingConfig"))
class HARVESTGAS_API UHarvestActionMappingConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Map of GameplayTag -> soft reference to a UDataAsset.
	 * Use TSoftObjectPtr to allow async loading.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mapping")
	TMap<FGameplayTag, TSoftObjectPtr<UHarvestableConfig>> TagMap;

	/**
	 * Returns the loaded UDataAsset for the given tag, or nullptr if not found.
	 * Loads synchronously if not already loaded.
	 */
	UFUNCTION(BlueprintCallable, Category="Mapping")
	UHarvestableConfig* GetDataForTag(FGameplayTag Tag) const;
};
