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

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHarvestHit(AActor* InstigatorActor);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDepleted(AActor* InstigatorActor);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnRespawned();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleHit(float NewRemaining, AActor* InstigatorActor);

	UFUNCTION()
	void HandleDepleted(AActor* InstigatorActor);

	UFUNCTION()
	void HandleRespawned();
};