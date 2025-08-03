// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "Engine/CurveTable.h"
#include "HarvestableConfig.generated.h"

UENUM(BlueprintType)
enum class EHarvestType : uint8
{
	Damageable,   // z.B. Baum, Stein (mit HP)
	InstantPickup // z.B. Stick, Berry, Pebble
};

UCLASS(BlueprintType)
class HARVESTGAS_API UHarvestableConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	EHarvestType HarvestType = EHarvestType::Damageable;

	// -- Shared --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FGameplayTag ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	float RespawnTime = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FGameplayTagContainer RequiredToolTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	TSubclassOf<UGameplayEffect> ExperienceGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTagContainer SourceTagsToApply;

	// -- Skill Configuration --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTag SkillAttributeTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSoftObjectPtr<UCurveTable> SkillMultiplierCurveTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FName SkillMultiplierCurveName;
	
	// -- Only for Damageable --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damageable", meta = (EditCondition = "HarvestType == EHarvestType::Damageable", EditConditionHides))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damageable", meta = (EditCondition = "HarvestType == EHarvestType::Damageable", EditConditionHides))
	float BaseDamage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damageable", meta = (EditCondition = "HarvestType == EHarvestType::Damageable", EditConditionHides))
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damageable", meta = (EditCondition = "HarvestType == EHarvestType::Damageable", EditConditionHides))
	TObjectPtr<UNiagaraSystem> HitNiagara;

	// -- Shared FX --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (EditCondition = "HarvestType == EHarvestType::Damageable", EditConditionHides))
	TObjectPtr<USoundBase> DepletedSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (EditCondition = "HarvestType == EHarvestType::Damageable", EditConditionHides))
	TObjectPtr<UNiagaraSystem> DepletedNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<USoundBase> RespawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UNiagaraSystem> RespawnNiagara;

	// -- Only for InstantPickup --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InstantPickup", meta = (EditCondition = "HarvestType == EHarvestType::InstantPickup", EditConditionHides))
	TObjectPtr<USoundBase> PickupSound;

	// Loot Info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loot")
	TMap<FGameplayTag, int32> LootItems;

public:
	// Helper function to get skill multiplier from curve table
	UFUNCTION(BlueprintCallable, Category = "Skill")
	float GetSkillMultiplierFromCurve(float SkillLevel) const;
};