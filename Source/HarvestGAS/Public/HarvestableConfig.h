// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "HarvestableConfig.generated.h"

UCLASS(BlueprintType)
class HARVESTGAS_API UHarvestableConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Harvest")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Harvest")
	float RespawnTime = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Harvest")
	FGameplayTag ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Harvest")
	FGameplayTagContainer RequiredToolTags;

	// FX / Sounds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FX")
	class UNiagaraSystem* HitNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FX")
	UNiagaraSystem* DepletedNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FX")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FX")
	USoundBase* DepletedSound;

	// Loot Info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loot")
	TMap<FGameplayTag, int32> LootItems;
};
