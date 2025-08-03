// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestableActor.h"

#include "Public/HarvestableComponent.h"


AHarvestableActor::AHarvestableActor()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;              // nie aus Relevanz kicken
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Harvestable = CreateDefaultSubobject<UHarvestableComponent>(TEXT("Harvestable"));
}