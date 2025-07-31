// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestableActor.h"

#include "Public/HarvestableComponent.h"


AHarvestableActor::AHarvestableActor()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	NetDormancy     = DORM_Never;        // Actor bleibt immer "awake"
	bAlwaysRelevant = true;              // nie aus Relevanz kicken
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Harvestable = CreateDefaultSubobject<UHarvestableComponent>(TEXT("Harvestable"));
}

void AHarvestableActor::BeginPlay()
{
	Super::BeginPlay();
	Harvestable->OnHit.AddDynamic(this, &AHarvestableActor::HandleHit);
	Harvestable->OnDepleted.AddDynamic(this, &AHarvestableActor::HandleDepleted);
	Harvestable->OnRespawned.AddDynamic(this, &AHarvestableActor::HandleRespawned);
}

void AHarvestableActor::HandleHit(float NewRemaining, AActor* InstigatorActor)
{
	BP_OnHarvestHit(InstigatorActor);
}

void AHarvestableActor::HandleDepleted(AActor* InstigatorActor)
{
	BP_OnDepleted(InstigatorActor);
}

void AHarvestableActor::HandleRespawned()
{
	BP_OnRespawned();
}