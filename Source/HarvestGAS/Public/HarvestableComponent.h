// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "HarvestableComponent.generated.h"

class UHarvestableConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHit, float, NewRemaining, AActor*, InstigatorActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDepleted, AActor*, InstigatorActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawned);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARVESTGAS_API UHarvestableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHarvestableComponent();
	
	// --- State ---
	UPROPERTY(ReplicatedUsing = OnRep_Remaining)
	float Remaining;

	UPROPERTY(ReplicatedUsing = OnRep_Depleted)
	bool bIsDepleted = false;

	// --- Events for Blueprint/Designer ---
	UPROPERTY(BlueprintAssignable)
	FOnHit OnHit;

	UPROPERTY(BlueprintAssignable)
	FOnDepleted OnDepleted;

	UPROPERTY(BlueprintAssignable)
	FOnRespawned OnRespawned;

	// --- Server API ---
	// Neue überladene Methode, die ActionConfig direkt akzeptiert
	UFUNCTION(Server, Reliable)
	void Server_ApplyHarvest(float Amount, AActor* InstigatorActor, UHarvestableConfig* ActionConfig);
	

	UFUNCTION(BlueprintCallable)
	bool CanBeHarvestedBy(AActor* InstigatorActor, UHarvestableConfig* ActionConfig) const;

	void Respawn();

	// --- FX ---
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitFX(AActor* InstigatorActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDepletedFX(AActor* InstigatorActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayRespawnFX();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetVisibilityAndCollision(bool bVisible);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ResourceType;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// -- Skill Configuration --

	void HandleDepleted(AActor* InstigatorActor);

	UFUNCTION()
	void OnRep_Remaining();

	UFUNCTION()
	void OnRep_Depleted();

	FTimerHandle RespawnTimer;

private:
	UPROPERTY()
	UHarvestableConfig* Config;
};