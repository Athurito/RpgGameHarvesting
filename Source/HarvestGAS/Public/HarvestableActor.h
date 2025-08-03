// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarvestableActor.generated.h"

class UHarvestableComponent;

UCLASS()
class HARVESTGAS_API AHarvestableActor : public AActor
{
	GENERATED_BODY()

public:
	AHarvestableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHarvestableComponent* Harvestable;
};