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
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_MiningTotalXP)
	FGameplayAttributeData MiningTotalXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, MiningTotalXP);
	
	/** Woodcutting */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_WoodcuttingLevel)
	FGameplayAttributeData WoodcuttingLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, WoodcuttingLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_WoodcuttingXP)
	FGameplayAttributeData WoodcuttingXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, WoodcuttingXP);

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_WoodcuttingTotalXP)
	FGameplayAttributeData WoodcuttingTotalXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, WoodcuttingTotalXP);

	/** Harvesting (Herbs) */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_HarvestingLevel)
	FGameplayAttributeData HarvestingLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, HarvestingLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_HarvestingXP)
	FGameplayAttributeData HarvestingXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, HarvestingXP);

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_HarvestingTotalXP)
	FGameplayAttributeData HarvestingTotalXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, HarvestingTotalXP);

	/** Fishing */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_FishingLevel)
	FGameplayAttributeData FishingLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, FishingLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_FishingXP)
	FGameplayAttributeData FishingXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, FishingXP);

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_FishingTotalXP)
	FGameplayAttributeData FishingTotalXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, FishingTotalXP);

	/** Skinning */
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_SkinningLevel)
	FGameplayAttributeData SkinningLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, SkinningLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_SkinningXP)
	FGameplayAttributeData SkinningXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, SkinningXP);
	
	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing=OnRep_SkinningTotalXP)
	FGameplayAttributeData SkinningTotalXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, SkinningTotalXP);
	
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
	void OnRep_MiningTotalXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_WoodcuttingLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_WoodcuttingXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_WoodcuttingTotalXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_HarvestingLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_HarvestingXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_HarvestingTotalXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_FishingLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_FishingXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_FishingTotalXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SkinningLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SkinningXP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SkinningTotalXP(const FGameplayAttributeData& OldValue);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};