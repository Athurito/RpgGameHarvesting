// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HarvestConfigRegistry.generated.h"

UCLASS(BlueprintType)
class HARVESTGAS_API UHarvestConfigRegistry : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable> SkillExperience;
};