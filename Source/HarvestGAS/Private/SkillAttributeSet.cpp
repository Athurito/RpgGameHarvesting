// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAttributeSet.h"

#include "Net/UnrealNetwork.h"


float USkillAttributeSet::XPForNextLevel(float CurrentLevel)
{
	return 100.f + (CurrentLevel * 50.f); // Beispiel: 100 XP + 50 pro Level
}

void USkillAttributeSet::CheckLevelUp()
{
	if (GetMiningXP() >= XPForNextLevel(GetMiningLevel()))
	{
		SetMiningXP(GetMiningXP() - XPForNextLevel(GetMiningLevel()));
		SetMiningLevel(GetMiningLevel() + 1.f);
	}

	if (GetWoodcuttingXP() >= XPForNextLevel(GetWoodcuttingLevel()))
	{
		SetWoodcuttingXP(GetWoodcuttingXP() - XPForNextLevel(GetWoodcuttingLevel()));
		SetWoodcuttingLevel(GetWoodcuttingLevel() + 1.f);
	}
}

void USkillAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkillAttributeSet, MiningLevel);
	DOREPLIFETIME(USkillAttributeSet, MiningXP);
	DOREPLIFETIME(USkillAttributeSet, WoodcuttingLevel);
	DOREPLIFETIME(USkillAttributeSet, WoodcuttingXP);
}

void USkillAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	CheckLevelUp();
}

void USkillAttributeSet::OnRep_MiningLevel(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(USkillAttributeSet, MiningLevel, OldValue); }
void USkillAttributeSet::OnRep_MiningXP(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(USkillAttributeSet, MiningXP, OldValue); }
void USkillAttributeSet::OnRep_WoodcuttingLevel(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(USkillAttributeSet, WoodcuttingLevel, OldValue); }
void USkillAttributeSet::OnRep_WoodcuttingXP(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(USkillAttributeSet, WoodcuttingXP, OldValue); }