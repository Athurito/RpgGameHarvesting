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
	// Mining
	UPROPERTY(BlueprintReadOnly, Category="Skills", ReplicatedUsing=OnRep_MiningLevel)
	FGameplayAttributeData MiningLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, MiningLevel);

	UPROPERTY(BlueprintReadOnly, Category="Skills", ReplicatedUsing=OnRep_MiningXP)
	FGameplayAttributeData MiningXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, MiningXP);

	// Woodcutting
	UPROPERTY(BlueprintReadOnly, Category="Skills", ReplicatedUsing=OnRep_WoodcuttingLevel)
	FGameplayAttributeData WoodcuttingLevel;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, WoodcuttingLevel);

	UPROPERTY(BlueprintReadOnly, Category="Skills", ReplicatedUsing=OnRep_WoodcuttingXP)
	FGameplayAttributeData WoodcuttingXP;
	ATTRIBUTE_ACCESSORS_BASIC(USkillAttributeSet, WoodcuttingXP);
	
	// Returns how much XP is needed for the next level (simple formula)
	UFUNCTION(BlueprintCallable, Category="Skills")
	static float XPForNextLevel(float CurrentLevel);

	// Called when XP changes -> check LevelUp
	void CheckLevelUp();


protected:
	UFUNCTION() void OnRep_MiningLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_MiningXP(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_WoodcuttingLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_WoodcuttingXP(const FGameplayAttributeData& OldValue);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};
