// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ToolItem.h"
#include "Net/UnrealNetwork.h"


// EquipmentComponent.cpp
UEquipmentComponent::UEquipmentComponent()
{
	SetIsReplicatedByDefault(true);
}
void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquipmentComponent, CurrentTool);
}
void UEquipmentComponent::Server_EquipTool_Implementation(UToolItem* Tool)
{
	// ggf. Validierung: Tool ist im Inventar etc.
	RemoveToolTags(CurrentTool);
	CurrentTool = Tool;
	ApplyToolTags(Tool);
}
void UEquipmentComponent::Server_UnequipTool_Implementation()
{
	RemoveToolTags(CurrentTool);
	CurrentTool = nullptr;
}
void UEquipmentComponent::OnRep_CurrentTool()
{
	// Auf Client dieselbe Logik ausführen
	RemoveToolTags(nullptr);              // entferne alte Tags
	ApplyToolTags(CurrentTool);
}
void UEquipmentComponent::ApplyToolTags(UToolItem* Tool)
{
	if (!Tool) return;
	if (auto* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		for (auto& Tag : Tool->ToolTags)
			ASC->AddLooseGameplayTag(Tag);
	}
}
void UEquipmentComponent::RemoveToolTags(UToolItem* Tool)
{
	if (auto* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		if (Tool)
			for (auto& Tag : Tool->ToolTags)
				ASC->RemoveLooseGameplayTag(Tag);
	}
}

