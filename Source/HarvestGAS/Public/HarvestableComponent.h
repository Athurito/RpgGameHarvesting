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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Remaining)
	float Remaining;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHarvestableConfig* Config;

	UPROPERTY(ReplicatedUsing=OnRep_Depleted)
	bool bIsDepleted = false;

	UFUNCTION(Server, Reliable)
	void Server_ApplyHarvest(float Amount, AActor* InstigatorActor, const FGameplayTagContainer& SourceTags);

	UPROPERTY(BlueprintAssignable) FOnHit OnHit;
	UPROPERTY(BlueprintAssignable) FOnDepleted OnDepleted;
	UPROPERTY(BlueprintAssignable) FOnRespawned OnRespawned;

	UFUNCTION(BlueprintCallable)
	bool CanBeHarvestedBy(AActor* InstigatorActor) const;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitFX(AActor* InstigatorActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDepletedFX(AActor* InstigatorActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayRespawnFX();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Remaining();

	UFUNCTION()
	void OnRep_Depleted();

	void HandleDepleted(AActor* InstigatorActor);
	void Respawn();

	FTimerHandle RespawnTimer;
};