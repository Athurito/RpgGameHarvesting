// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAttributeSet.h"

#include "HarvestConfigRegistry.h"
#include "Net/UnrealNetwork.h"


float USkillAttributeSet::XPForNextLevel(float CurrentLevel, FName CurveName)
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UHarvestConfigRegistry* harvest = Cast<UHarvestConfigRegistry>(GameInstance);
	return harvest->SkillExperience->FindCurve(CurveName, FString())->Eval(CurrentLevel);
}

void USkillAttributeSet::CheckLevelUp()
{

	auto TryLevelUp = [&](FGameplayAttributeData& XP, FGameplayAttributeData& Level, FName CurveName)
	{
		float Current = Level.GetCurrentValue();
		float Needed  = XPForNextLevel(Current, CurveName);
		// Solange genug XP für nächstes Level vorhanden ist, weiter leveln
		while (XP.GetCurrentValue() >= Needed)
		{
			XP.SetCurrentValue(XP.GetCurrentValue() - Needed);
			Current += 1.f;
			Level.SetCurrentValue(Current);
			Needed = XPForNextLevel(Current, CurveName);
		}
	};

	TryLevelUp(MiningXP,        MiningLevel, "Mining");
	TryLevelUp(WoodcuttingXP,   WoodcuttingLevel, "Woodcutting");
	TryLevelUp(HarvestingXP,    HarvestingLevel, "Harvesting");
	TryLevelUp(FishingXP,       FishingLevel, "Fishing");
	TryLevelUp(SkinningXP,      SkinningLevel, "Skinning");
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

// Einfache Macro-Hilfe für RepNotifies
#define REP_NOTIFY(Prop) \
void USkillAttributeSet::OnRep_##Prop(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(USkillAttributeSet, Prop, OldValue); }

REP_NOTIFY(MiningLevel)
REP_NOTIFY(MiningXP)
REP_NOTIFY(WoodcuttingLevel)
REP_NOTIFY(WoodcuttingXP)
REP_NOTIFY(HarvestingLevel)
REP_NOTIFY(HarvestingXP)
REP_NOTIFY(FishingLevel)
REP_NOTIFY(FishingXP)
REP_NOTIFY(SkinningLevel)
REP_NOTIFY(SkinningXP)

#undef REP_NOTIFY