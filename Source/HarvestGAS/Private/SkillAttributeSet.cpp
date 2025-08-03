// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAttributeSet.h"

#include "GameplayEffectExtension.h"
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
	auto TryLevelUp = [&](FGameplayAttributeData& XP, FGameplayAttributeData& Level, FGameplayAttributeData& TotalXP, FName CurveName)
	{
		float Current = Level.GetCurrentValue();
		float Needed  = XPForNextLevel(Current, CurveName);
		// Solange genug XP für nächstes Level vorhanden ist, weiter leveln
		while (XP.GetCurrentValue() >= Needed)
		{
			XP.SetCurrentValue(XP.GetCurrentValue() - Needed);
			TotalXP.SetCurrentValue(TotalXP.GetCurrentValue() + Needed);
			Current += 1.f;
			Level.SetCurrentValue(Current);
			Needed = XPForNextLevel(Current, CurveName);
		}
	};

	TryLevelUp(MiningXP,        MiningLevel,        MiningTotalXP,        "Mining");
	TryLevelUp(WoodcuttingXP,   WoodcuttingLevel,   WoodcuttingTotalXP,   "Woodcutting");
	TryLevelUp(HarvestingXP,    HarvestingLevel,    HarvestingTotalXP,    "Harvesting");
	TryLevelUp(FishingXP,       FishingLevel,       FishingTotalXP,       "Fishing");
	TryLevelUp(SkinningXP,      SkinningLevel,      SkinningTotalXP,      "Skinning");
}

void USkillAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkillAttributeSet, MiningLevel);
	DOREPLIFETIME(USkillAttributeSet, MiningXP);
	DOREPLIFETIME(USkillAttributeSet, MiningTotalXP);
	DOREPLIFETIME(USkillAttributeSet, WoodcuttingLevel);
	DOREPLIFETIME(USkillAttributeSet, WoodcuttingXP);
	DOREPLIFETIME(USkillAttributeSet, WoodcuttingTotalXP);
	DOREPLIFETIME(USkillAttributeSet, HarvestingLevel);
	DOREPLIFETIME(USkillAttributeSet, HarvestingXP);
	DOREPLIFETIME(USkillAttributeSet, HarvestingTotalXP);
	DOREPLIFETIME(USkillAttributeSet, FishingLevel);
	DOREPLIFETIME(USkillAttributeSet, FishingXP);
	DOREPLIFETIME(USkillAttributeSet, FishingTotalXP);
	DOREPLIFETIME(USkillAttributeSet, SkinningLevel);
	DOREPLIFETIME(USkillAttributeSet, SkinningXP);
	DOREPLIFETIME(USkillAttributeSet, SkinningTotalXP);
}

void USkillAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// Wenn XP hinzugefügt wird, auch TotalXP aktualisieren
	if (Data.EvaluatedData.Attribute == GetMiningXPAttribute())
	{
		SetMiningTotalXP(GetMiningTotalXP() + Data.EvaluatedData.Magnitude);
	}
	else if (Data.EvaluatedData.Attribute == GetWoodcuttingXPAttribute())
	{
		SetWoodcuttingTotalXP(GetWoodcuttingTotalXP() + Data.EvaluatedData.Magnitude);
	}
	else if (Data.EvaluatedData.Attribute == GetHarvestingXPAttribute())
	{
		SetHarvestingTotalXP(GetHarvestingTotalXP() + Data.EvaluatedData.Magnitude);
	}
	else if (Data.EvaluatedData.Attribute == GetFishingXPAttribute())
	{
		SetFishingTotalXP(GetFishingTotalXP() + Data.EvaluatedData.Magnitude);
	}
	else if (Data.EvaluatedData.Attribute == GetSkinningXPAttribute())
	{
		SetSkinningTotalXP(GetSkinningTotalXP() + Data.EvaluatedData.Magnitude);
	}
	
	CheckLevelUp();
}

// Einfache Macro-Hilfe für RepNotifies
#define REP_NOTIFY(Prop) \
void USkillAttributeSet::OnRep_##Prop(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(USkillAttributeSet, Prop, OldValue); }

REP_NOTIFY(MiningLevel)
REP_NOTIFY(MiningXP)
REP_NOTIFY(MiningTotalXP)
REP_NOTIFY(WoodcuttingLevel)
REP_NOTIFY(WoodcuttingXP)
REP_NOTIFY(WoodcuttingTotalXP)
REP_NOTIFY(HarvestingLevel)
REP_NOTIFY(HarvestingXP)
REP_NOTIFY(HarvestingTotalXP)
REP_NOTIFY(FishingLevel)
REP_NOTIFY(FishingXP)
REP_NOTIFY(FishingTotalXP)
REP_NOTIFY(SkinningLevel)
REP_NOTIFY(SkinningXP)
REP_NOTIFY(SkinningTotalXP)

#undef REP_NOTIFY