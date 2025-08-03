// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SkillAttributeSet.generated.h"

UCLASS()
class HARVESTGAS_API USkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	/** Curve für XP-Bedarf pro Level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP Progression")
	FRuntimeFloatCurve XPForNextLevelCurve;

	/** Mining */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_MiningLevel)
	FGameplayAttributeData MiningLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, MiningLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_MiningXP)
	FGameplayAttributeData MiningXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, MiningXP);
	
	/** Woodcutting */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_WoodcuttingLevel)
	FGameplayAttributeData WoodcuttingLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, WoodcuttingLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_WoodcuttingXP)
	FGameplayAttributeData WoodcuttingXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, WoodcuttingXP);

	/** Harvesting (Herbs) */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_HarvestingLevel)
	FGameplayAttributeData HarvestingLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, HarvestingLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_HarvestingXP)
	FGameplayAttributeData HarvestingXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, HarvestingXP);

	/** Fishing */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_FishingLevel)
	FGameplayAttributeData FishingLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, FishingLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_FishingXP)
	FGameplayAttributeData FishingXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, FishingXP);

	/** Skinning */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_SkinningLevel)
	FGameplayAttributeData SkinningLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, SkinningLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_SkinningXP)
	FGameplayAttributeData SkinningXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, SkinningXP);
	
	// Returns how much XP is needed for the next level (simple formula)
	UFUNCTION(BlueprintCallable, Category="Skills")
	float XPForNextLevel(float CurrentLevel, FName CurveName);
	

	// Called when XP changes -> check LevelUp
	void CheckLevelUp();


protected:
	/** OnRep Callbacks */
	UFUNCTION()
	void OnRep_MiningLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MiningXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_WoodcuttingLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_WoodcuttingXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_HarvestingLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_HarvestingXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_FishingLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_FishingXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SkinningLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SkinningXP(const FGameplayAttributeData& OldValue);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};