// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/HarvestActionMappingConfig.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HarvestConfigRegistry.generated.h"

UCLASS(BlueprintType)
class HARVESTGAS_API UHarvestConfigRegistry : public UGameInstance
{
	GENERATED_BODY()

public:
	/** Returns the UDataAsset associated with a GameplayTag, or nullptr. */
	UFUNCTION(Category="Mapping")
	UHarvestableConfig* GetForTag(FGameplayTag Tag) const;
	
	UPROPERTY(EditDefaultsOnly)
	UHarvestActionMappingConfig* MappingAsset = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable> SkillExperience;
};