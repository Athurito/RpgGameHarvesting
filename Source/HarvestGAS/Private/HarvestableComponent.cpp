// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestableComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HarvestableConfig.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UHarvestableComponent::UHarvestableComponent()
{
	SetIsReplicatedByDefault(true);
}

void UHarvestableComponent::BeginPlay()
{
    Super::BeginPlay();
    Remaining = Config ? Config->MaxHealth : 100.f;
}

bool UHarvestableComponent::CanBeHarvestedBy(AActor* InstigatorActor) const
{
    if (!Config || !InstigatorActor) return false;

    UAbilitySystemComponent* InstigatorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);
    if (!InstigatorASC) return false;

    const FGameplayTagContainer& RequiredTags = Config->RequiredToolTags;
    if (!RequiredTags.IsEmpty() && !InstigatorASC->HasAllMatchingGameplayTags(RequiredTags))
    {
        return false; // Spieler hat nicht die benötigten Tool-Tags (z.B. Tool.Axe)
    }

    return true;
}

void UHarvestableComponent::Server_ApplyHarvest_Implementation(float Amount, AActor* InstigatorActor, const FGameplayTagContainer& SourceTags)
{
    if (!Config || bIsDepleted) return;


    // Prüfen, ob der Spieler das richtige Tool/Tag besitzt
    if (!CanBeHarvestedBy(InstigatorActor))
    {
        return; // Abbrechen, Spieler kann diesen Node nicht ernten
    }

    
    Remaining = FMath::Clamp(Remaining - Amount, 0.f, Config->MaxHealth);
    OnHit.Broadcast(Remaining, InstigatorActor);
    Multicast_PlayHitFX(InstigatorActor);

    if (Remaining <= 0.f)
    {
        bIsDepleted = true;
        HandleDepleted(InstigatorActor);
        if (Config->RespawnTime > 0.f)
        {
            GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &UHarvestableComponent::Respawn, Config->RespawnTime, false);
        }
    }
}

void UHarvestableComponent::HandleDepleted(AActor* InstigatorActor)
{
    OnDepleted.Broadcast(InstigatorActor);
    Multicast_PlayDepletedFX(InstigatorActor);
}

void UHarvestableComponent::Respawn()
{
    bIsDepleted = false;
    Remaining = Config ? Config->MaxHealth : 100.f;
    OnRespawned.Broadcast();
    Multicast_PlayRespawnFX();
}

void UHarvestableComponent::Multicast_PlayHitFX_Implementation(AActor* Instigator)
{
    if (Config && Config->HitNiagara)
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Config->HitNiagara, GetOwner()->GetActorLocation());

    if (Config && Config->HitSound)
        UGameplayStatics::PlaySoundAtLocation(this, Config->HitSound, GetOwner()->GetActorLocation());
}

void UHarvestableComponent::Multicast_PlayDepletedFX_Implementation(AActor* Instigator)
{
    if (Config && Config->DepletedNiagara)
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Config->DepletedNiagara, GetOwner()->GetActorLocation());

    if (Config && Config->DepletedSound)
        UGameplayStatics::PlaySoundAtLocation(this, Config->DepletedSound, GetOwner()->GetActorLocation());
}

void UHarvestableComponent::Multicast_PlayRespawnFX_Implementation()
{
    // Optional respawn FX
}

void UHarvestableComponent::OnRep_Remaining() { /* UI Update etc. */ }
void UHarvestableComponent::OnRep_Depleted() { /* Mesh/Collision Updates */ }

void UHarvestableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHarvestableComponent, Remaining);
    DOREPLIFETIME(UHarvestableComponent, bIsDepleted);
}