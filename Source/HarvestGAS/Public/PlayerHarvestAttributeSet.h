// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PlayerHarvestAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class HARVESTGAS_API UPlayerHarvestAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Harvest", ReplicatedUsing=OnRep_HarvestPower)
	FGameplayAttributeData HarvestPower;
	ATTRIBUTE_ACCESSORS(UPlayerHarvestAttributeSet, HarvestPower)

	UPROPERTY(BlueprintReadOnly, Category="Harvest", ReplicatedUsing=OnRep_YieldBonus)
	FGameplayAttributeData YieldBonus;
	ATTRIBUTE_ACCESSORS(UPlayerHarvestAttributeSet, YieldBonus)

	// --- UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_HarvestPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_YieldBonus(const FGameplayAttributeData& OldValue);
};
