// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ToolItem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HARVESTGAS_API UToolItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Tags, die beim Equipten hinzugefügt werden (z.B. "Tool.Pickaxe", "Tool.Axe" etc.)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool")
	FGameplayTagContainer ToolTags;

	// (Optional) Mesh, Icon, Skill-Typ, etc.
};
