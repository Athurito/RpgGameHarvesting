// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PlayerHarvestAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UPlayerHarvestAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerHarvestAttributeSet, HarvestPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerHarvestAttributeSet, YieldBonus,  COND_None, REPNOTIFY_Always);
}

void UPlayerHarvestAttributeSet::OnRep_HarvestPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerHarvestAttributeSet, HarvestPower, OldValue);
}

void UPlayerHarvestAttributeSet::OnRep_YieldBonus(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerHarvestAttributeSet, YieldBonus, OldValue);
}