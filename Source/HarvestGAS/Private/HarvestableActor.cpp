// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestableActor.h"

#include "Public/HarvestableComponent.h"


AHarvestableActor::AHarvestableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Harvestable = CreateDefaultSubobject<UHarvestableComponent>(TEXT("Harvestable"));
	bReplicates = true;
}

void AHarvestableActor::BeginPlay()
{
	Super::BeginPlay();
	Harvestable->OnDepleted.AddDynamic(this, &AHarvestableActor::OnDepleted);
}

void AHarvestableActor::OnDepleted()
{
	// Optional zusätzliche Logik (FX, Sounds)
}

