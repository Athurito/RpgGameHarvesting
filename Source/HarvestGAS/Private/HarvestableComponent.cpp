// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestableComponent.h"

#include "Net/UnrealNetwork.h"


UHarvestableComponent::UHarvestableComponent()
{
	SetIsReplicatedByDefault(true);
}

void UHarvestableComponent::BeginPlay()
{
	Super::BeginPlay();
	Remaining = FMath::Clamp(Remaining, 0.f, MaxAmount);
}

// void UHarvestableComponent::HandleDepleted(AActor* InstigatorActor)
// {
// 	OnDepleted.Broadcast();
//
// 	// Einfach: Mesh ausblenden, Actor bleibt
// 	if (AActor* Owner = GetOwner())
// 	{
// 		Owner->Destroy();
// 	}
//
// 	// Loot / Respawn kann hier hinzugefügt werden
// }

void UHarvestableComponent::OnRep_Remaining()
{
	// UI / FX aktualisieren
}

void UHarvestableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHarvestableComponent, Remaining);
}

void UHarvestableComponent::Server_ApplyHarvest_Implementation(float Amount, AActor* InstigatorActor)
{
	if (Remaining <= 0.f) return;

	Remaining = FMath::Clamp(Remaining - Amount, 0.f, MaxAmount);

	if (Remaining <= 0.f && !bIsDepleted)
	{
		bIsDepleted = true;

		// Sofort auf dem Server anwenden (für den lokalen Spieler/ListenServer)
		Multicast_ApplyDepletedState();   // oder: OnRep_Depleted() manuell auf dem Server callen
	}
}

void UHarvestableComponent::OnRep_Depleted()
{
	if (bIsDepleted)
	{
		Multicast_ApplyDepletedState(); // sicherstellen, dass Clients die gleiche Visual-Logik bekommen
	}
}

void UHarvestableComponent::Multicast_ApplyDepletedState_Implementation()
{
	OnDepleted.Broadcast();
	if (AActor* Owner = GetOwner())
	{
		Owner->Destroy();
	}
}