// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "HarvestableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHarvestableDepleted);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARVESTGAS_API UHarvestableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHarvestableComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Remaining)
	float Remaining = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAmount = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ResourceType; // e.g. Resource.Wood

	UFUNCTION(Server, Reliable)
	void Server_ApplyHarvest(float Amount, AActor* InstigatorActor);

	UPROPERTY(BlueprintAssignable)
	FOnHarvestableDepleted OnDepleted;


protected:
	UFUNCTION()
	void OnRep_Remaining();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_Depleted)
	bool bIsDepleted = false;

	UFUNCTION()
	void OnRep_Depleted();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyDepletedState();

private:
	// void HandleDepleted(AActor* InstigatorActor);
};
