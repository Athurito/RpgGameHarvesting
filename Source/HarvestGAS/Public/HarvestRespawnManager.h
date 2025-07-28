// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarvestableComponent.h"
#include "GameFramework/Actor.h"
#include "HarvestRespawnManager.generated.h"


USTRUCT()
struct FPendingRespawnEntry
{
	GENERATED_BODY()

	UPROPERTY()
	UHarvestableComponent* Harvestable = nullptr;
	float RespawnTime; // in seconds
	float ExpireTimestamp;
};

UCLASS()
class HARVESTGAS_API AHarvestRespawnManager : public AActor
{
	GENERATED_BODY()

public:
	AHarvestRespawnManager();
	static AHarvestRespawnManager* Get(UWorld* World);

	void RegisterForRespawn(UHarvestableComponent* Harvestable, float InSeconds);

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<FPendingRespawnEntry> PendingRespawns;
};
