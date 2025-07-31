// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HarvestRespawnSubsystem.generated.h"

/**
 * 
 */


class UHarvestableComponent;

USTRUCT()
struct FRespawnEntry
{
	GENERATED_BODY()

	// Schwacher Pointer, damit wir Einträge automatisch säubern, wenn Actor zerstört wird
	UPROPERTY()
	UHarvestableComponent* Component;

	// Zeitpunkt (in GetWorld()->GetTimeSeconds()), ab dem respawnt werden darf
	float RespawnTime = 0.f;
};

UCLASS()
class HARVESTGAS_API UHarvestRespawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		if (const UWorld* World = Cast<UWorld>(Outer))
		{
			// nur in Dedicated-Server oder Listen-Server
			return World->GetNetMode() != NM_Client;
		}
		return false;
	}
	// wird automatisch vom Engine Lifetime-Management aufgerufen
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Registriert eine Komponente für Respawn in DelaySekunden */
	void RegisterForRespawn(UHarvestableComponent* Comp, float DelaySekunden);

private:
	/** Alle offenen Respawn‑Einträge */
	TArray<FRespawnEntry> PendingRespawns;

	/** Timer‑Handle für den periodischen Check */
	FTimerHandle TimerHandle_CheckRespawns;

	/** Callback, das jede Sekunde läuft und Einträge abarbeitet */
	void CheckRespawns();
};
