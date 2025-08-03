// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/HarvestableConfig.h"
#include "Engine/CurveTable.h"

float UHarvestableConfig::GetSkillMultiplierFromCurve(float SkillLevel) const
{
	if (!SkillMultiplierCurveTable.IsValid() || SkillMultiplierCurveName.IsNone())
	{
		// Fallback auf den alten festen Wert
		return 0.05f;
	}

	UCurveTable* CurveTable = SkillMultiplierCurveTable.LoadSynchronous();
	if (!CurveTable)
	{
		return 0.05f;
	}

	FRealCurve* Curve = CurveTable->FindCurve(SkillMultiplierCurveName, TEXT(""));
	if (!Curve)
	{
		return 0.05f;
	}

	return Curve->Eval(SkillLevel);
}