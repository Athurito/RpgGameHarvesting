// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"


class UToolItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARVESTGAS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UEquipmentComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Server-RPCs fürs Equip/Unequip
	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_EquipTool(UToolItem* Tool);
	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_UnequipTool();

	// Repliziertes Feld; Clients reagieren in OnRep
	UPROPERTY(ReplicatedUsing=OnRep_CurrentTool)
	UToolItem* CurrentTool;

protected:
	UFUNCTION()
	void OnRep_CurrentTool();

	void ApplyToolTags(UToolItem* Tool);
	void RemoveToolTags(UToolItem* Tool);
};
