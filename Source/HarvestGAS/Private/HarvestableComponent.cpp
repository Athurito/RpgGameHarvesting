// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestableComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HarvestableConfig.h"
#include "HarvestRespawnSubsystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UHarvestableComponent::UHarvestableComponent()
{
    SetIsReplicatedByDefault(true);
    PrimaryComponentTick.bCanEverTick = false;
}

void UHarvestableComponent::BeginPlay()
{
    Super::BeginPlay();
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


bool UHarvestableComponent::CanBeHarvestedBy(AActor* InstigatorActor, UHarvestableConfig* ActionConfig) const
{
    if (!ActionConfig || !InstigatorActor) return false;

    UAbilitySystemComponent* InstigatorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);
    if (!InstigatorASC) return false;

    const FGameplayTagContainer& Required = ActionConfig->RequiredToolTags;

    if (!Required.IsEmpty() && !InstigatorASC->HasAllMatchingGameplayTags(Required))
    {
        return false;
    }

    return true;
}

void UHarvestableComponent::Server_ApplyHarvest_Implementation(float Amount, AActor* InstigatorActor, UHarvestableConfig* ActionConfig)
{
    if (!ActionConfig || bIsDepleted) return;
    
    Config = ActionConfig;
    
    if (!CanBeHarvestedBy(InstigatorActor, ActionConfig))
        return;

    // --- Instant Pickup (Sticks etc.)
    if (ActionConfig->HarvestType == EHarvestType::InstantPickup)
    {
        bIsDepleted = true;
        HandleDepleted(InstigatorActor);
        if (ActionConfig->RespawnTime > 0.f)
        {
            if (UWorld* World = GetWorld())
            {
                if (auto* Sub = World->GetSubsystem<UHarvestRespawnSubsystem>())
                {
                    Sub->RegisterForRespawn(this, ActionConfig->RespawnTime);
                }
            }
        }
        return;
    }

    // --- Damageable
    Remaining = FMath::Clamp(Remaining - Amount, 0.f, ActionConfig->MaxHealth);
    OnHit.Broadcast(Remaining, InstigatorActor);
    Multicast_PlayHitFX(InstigatorActor);

    if (Remaining <= 0.f)
    {
        bIsDepleted = true;
        HandleDepleted(InstigatorActor);

        if (ActionConfig->RespawnTime > 0.f)
        {
            if (UWorld* World = GetWorld())
            {
                if (auto* Sub = World->GetSubsystem<UHarvestRespawnSubsystem>())
                {
                    Sub->RegisterForRespawn(this, ActionConfig->RespawnTime);
                }
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
    bIsDepleted = false;
    Remaining   = Config ? Config->MaxHealth : 100.f;

    OnRespawned.Broadcast();

    // // wecke Dormant‑Actor kurz auf
    // GetOwner()->SetNetDormancy(DORM_Awake);

    // spiele FX und setze sichtbar
    Multicast_PlayRespawnFX();
    Multicast_SetVisibilityAndCollision(true);

    // FORCIERT Sofort‑Replikation an alle Clients
    GetOwner()->ForceNetUpdate();
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
void UHarvestableComponent::OnRep_Depleted()
{
    if (AActor* Owner = GetOwner())
    {
        bool bVisible = !bIsDepleted;
        TArray<UPrimitiveComponent*> PrimitiveComps;
        Owner->GetComponents<UPrimitiveComponent>(PrimitiveComps);

        for (UPrimitiveComponent* Prim : PrimitiveComps)
        {
            Prim->SetCollisionEnabled(
                bVisible 
                    ? ECollisionEnabled::QueryAndPhysics 
                    : ECollisionEnabled::NoCollision
            );
            Prim->SetVisibility(bVisible);
        }
    }
}

void UHarvestableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHarvestableComponent, Remaining);
    DOREPLIFETIME(UHarvestableComponent, bIsDepleted);
}