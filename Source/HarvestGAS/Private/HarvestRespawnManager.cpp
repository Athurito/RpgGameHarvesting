// Fill out your copyright notice in the Description page of Project Settings.


#include "HarvestRespawnManager.h"

#include "EngineUtils.h"
#include "HarvestableComponent.h"


void AHarvestRespawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

AHarvestRespawnManager::AHarvestRespawnManager()
{
	if (!HasAuthority())
	{
		AActor::SetActorTickEnabled(false);
		return;
	}
	PrimaryActorTick.bCanEverTick = true;
	AActor::SetActorTickEnabled(true);
	SetReplicates(false); // nur Server
}

AHarvestRespawnManager* AHarvestRespawnManager::Get(UWorld* World)
{
	// for (TActorIterator<AHarvestRespawnManager> It(World); It; ++It)
	// 	return *It;
	//
	// // Wenn keiner existiert: erzeugen
	// FActorSpawnParameters Params;
	// Params.bAllowDuringConstructionScript = false;
	// return World->SpawnActor<AHarvestRespawnManager>(Params);

	for (TActorIterator<AHarvestRespawnManager> It(World); It; ++It)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Found existing"));
		return *It;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AHarvestRespawnManager* Spawned = World->SpawnActor<AHarvestRespawnManager>(Params);

	if (Spawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Spawned new instance"));
	}

	return Spawned;
}

void AHarvestRespawnManager::RegisterForRespawn(UHarvestableComponent* Harvestable, float InSeconds)
{
	if (!Harvestable) return;
	UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Registered harvestable for %.2f seconds"), InSeconds);

	FPendingRespawnEntry Entry;
	Entry.Harvestable = Harvestable;
	Entry.RespawnTime = InSeconds;
	Entry.ExpireTimestamp = GetWorld()->GetRealTimeSeconds() + InSeconds;

	UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Registered %s for %.2f sec (expire @ %.2f)"),
		*Harvestable->GetOwner()->GetName(),
		InSeconds,
		Entry.ExpireTimestamp);

	PendingRespawns.Add(Entry);
}

void AHarvestRespawnManager::Tick(float DeltaSeconds)
{

	if (!HasAuthority())
	{
		return; // Nur Server darf respawnen!
	}
	
	if (PendingRespawns.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Checking %d pending entries..."), PendingRespawns.Num());
	}

	const float Now = GetWorld()->GetRealTimeSeconds();

	for (int32 i = PendingRespawns.Num() - 1; i >= 0; --i)
	{
		
		auto& Entry = PendingRespawns[i];
		UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Now: %.2f | Expires: %.2f | Remaining: %.2f"),
Now, Entry.ExpireTimestamp, Entry.ExpireTimestamp - Now);
		
		if (!Entry.Harvestable)
		{
			PendingRespawns.RemoveAt(i);
			continue;
		}

		if (Entry.ExpireTimestamp <= Now || true)
		{
			UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Respawning %s"), *Entry.Harvestable->GetOwner()->GetName());

			Entry.Harvestable->Respawn();
			PendingRespawns.RemoveAt(i);
		}
	
	}
}
