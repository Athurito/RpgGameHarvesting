// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestableComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HarvestableConfig.h"
#include "HarvestRespawnManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UHarvestableComponent::UHarvestableComponent()
{
    SetIsReplicatedByDefault(true);
    PrimaryComponentTick.bCanEverTick = false;
}

void UHarvestableComponent::Multicast_SetVisibilityAndCollision_Implementation(bool bVisible)
{
    if (AActor* Owner = GetOwner())
    {
        TArray<UPrimitiveComponent*> PrimitiveComps;
        Owner->GetComponents<UPrimitiveComponent>(PrimitiveComps);

        for (UPrimitiveComponent* Prim : PrimitiveComps)
        {
            Prim->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
            Prim->SetVisibility(bVisible);
        }
    }
}

void UHarvestableComponent::BeginPlay()
{
    Super::BeginPlay();
    Remaining = Config ? Config->MaxHealth : 100.f;
}

bool UHarvestableComponent::CanBeHarvestedBy(AActor* InstigatorActor, const FGameplayTagContainer& SourceTags) const
{
    if (!Config || !InstigatorActor) return false;

    UAbilitySystemComponent* InstigatorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);
    if (!InstigatorASC) return false;

    const FGameplayTagContainer& Required = Config->RequiredToolTags;

    if (!Required.IsEmpty() && !InstigatorASC->HasAllMatchingGameplayTags(Required))
    {
        return false;
    }

    return true;
}

void UHarvestableComponent::Server_ApplyHarvest_Implementation(float Amount, AActor* InstigatorActor, const FGameplayTagContainer& SourceTags)
{
    if (!Config || bIsDepleted) return;

    if (!CanBeHarvestedBy(InstigatorActor, SourceTags))
        return;

    // --- Instant Pickup (Sticks etc.)
    if (Config->HarvestType == EHarvestType::InstantPickup)
    {
        bIsDepleted = true;
        HandleDepleted(InstigatorActor);
        if (Config->RespawnTime > 0.f)
        {
            AHarvestRespawnManager* Manager = AHarvestRespawnManager::Get(GetWorld());
            if (Manager)
            {
                // Manager->RegisterForRespawn(this, Config->RespawnTime);
                Manager->RegisterForRespawn(this, 1);
            }
        }
        return;
    }

    // --- Damageable
    Remaining = FMath::Clamp(Remaining - Amount, 0.f, Config->MaxHealth);
    OnHit.Broadcast(Remaining, InstigatorActor);
    Multicast_PlayHitFX(InstigatorActor);

    if (Remaining <= 0.f)
    {
        bIsDepleted = true;
        HandleDepleted(InstigatorActor);

        if (Config->RespawnTime > 0.f)
        {
            AHarvestRespawnManager* Manager = AHarvestRespawnManager::Get(GetWorld());
            if (Manager)
            {
                Manager->RegisterForRespawn(this, Config->RespawnTime);
            }
        }
    }
}

void UHarvestableComponent::HandleDepleted(AActor* InstigatorActor)
{
    OnDepleted.Broadcast(InstigatorActor);
    Multicast_PlayDepletedFX(InstigatorActor);
    
    Multicast_SetVisibilityAndCollision(false);

}

void UHarvestableComponent::Respawn()
{
    UE_LOG(LogTemp, Warning, TEXT("[RespawnManager] Respawning "));

    UE_LOG(LogTemp, Warning, TEXT("[Respawn] Called on: %s | Role: %d"),
    *GetOwner()->GetName(),
    GetOwner()->GetLocalRole());
    bIsDepleted = false;
    Remaining = Config ? Config->MaxHealth : 100.f;
    OnRespawned.Broadcast();
    Multicast_PlayRespawnFX();
    
    Multicast_SetVisibilityAndCollision(true);

}

void UHarvestableComponent::Multicast_PlayHitFX_Implementation(AActor* InstigatorActor)
{
    if (Config && Config->HitNiagara)
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Config->HitNiagara, GetOwner()->GetActorLocation());

    if (Config && Config->HitSound)
        UGameplayStatics::PlaySoundAtLocation(this, Config->HitSound, GetOwner()->GetActorLocation());
}

void UHarvestableComponent::Multicast_PlayDepletedFX_Implementation(AActor* InstigatorActor)
{
    if (Config && Config->DepletedNiagara)
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Config->DepletedNiagara, GetOwner()->GetActorLocation());

    if (Config && Config->DepletedSound)
        UGameplayStatics::PlaySoundAtLocation(this, Config->DepletedSound, GetOwner()->GetActorLocation());
}

void UHarvestableComponent::Multicast_PlayRespawnFX_Implementation()
{
    if (Config && Config->RespawnNiagara)
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Config->RespawnNiagara, GetOwner()->GetActorLocation());

    if (Config && Config->RespawnSound)
        UGameplayStatics::PlaySoundAtLocation(this, Config->RespawnSound, GetOwner()->GetActorLocation());
}

void UHarvestableComponent::OnRep_Remaining() { /* UI etc. */ }
void UHarvestableComponent::OnRep_Depleted() { /* visibility client-side */ }

void UHarvestableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHarvestableComponent, Remaining);
    DOREPLIFETIME(UHarvestableComponent, bIsDepleted);
}