// Fill out your copyright notice in the Description page of Project Settings.


#include "HarvestRespawnSubsystem.h"

#include "HarvestableComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UHarvestRespawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// einmal pro Sekunde prüfen
	GetWorld()->GetTimerManager()
		.SetTimer(TimerHandle_CheckRespawns, this, &UHarvestRespawnSubsystem::CheckRespawns, 1.f, true);
}

void UHarvestRespawnSubsystem::Deinitialize()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CheckRespawns);
	}
	Super::Deinitialize();
}

void UHarvestRespawnSubsystem::RegisterForRespawn(UHarvestableComponent* Comp, float DelaySekunden)
{
	if (!Comp) return;

	FRespawnEntry Entry;
	Entry.Component   = Comp;
	Entry.RespawnTime = GetWorld()->GetTimeSeconds() + DelaySekunden;
	PendingRespawns.Add(Entry);
}

void UHarvestRespawnSubsystem::CheckRespawns()
{
	float Now = GetWorld()->GetTimeSeconds();
	// rückwärts iterieren, um sicher zu löschen
	for (int32 i = PendingRespawns.Num() - 1; i >= 0; --i)
	{
		auto& E = PendingRespawns[i];
		if (!E.Component)
		{
			PendingRespawns.RemoveAt(i);
			continue;
		}

		if (Now >= E.RespawnTime)
		{
			E.Component->Respawn();      // server-seitig, löst Multicast für Clients aus
			PendingRespawns.RemoveAt(i);
		}
	}
}