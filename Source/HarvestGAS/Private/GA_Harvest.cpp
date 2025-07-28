// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GA_Harvest.h"

#include "HarvestableConfig.h"
#include "SkillAttributeSet.h"
#include "GameFramework/Character.h"
#include "Public/HarvestableComponent.h"
#include "Public/PlayerHarvestAttributeSet.h"


void UGA_Harvest::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character) return;

    FVector Start = Character->GetActorLocation() + Character->GetActorForwardVector() * 50.f;
    FVector End = Start + Character->GetActorForwardVector() * TraceDistance;

    FHitResult Hit;
    if (Character->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            if (UHarvestableComponent* Harvestable = HitActor->FindComponentByClass<UHarvestableComponent>())
            {
                float FinalDamage = BaseHarvestDamage;

                if (const USkillAttributeSet* Skills = ActorInfo->AbilitySystemComponent->GetSet<USkillAttributeSet>())
                {
                    if (Harvestable->Config->ResourceType.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Resource.Stone"))))
                    {
                        FinalDamage *= (1.f + Skills->GetMiningLevel() * 0.05f);
                        
                        if (MiningXP_GE)
                        {
                            ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(+
                                MiningXP_GE->GetDefaultObject<UGameplayEffect>(), 1.f,
                                ActorInfo->AbilitySystemComponent->MakeEffectContext());
                        }
                    }
                    else if (Harvestable->Config->ResourceType.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Resource.Wood"))))
                    {
                        FinalDamage *= (1.f + Skills->GetWoodcuttingLevel() * 0.05f);
                        if (WoodcuttingXP_GE)
                        {
                            ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(
                                WoodcuttingXP_GE->GetDefaultObject<UGameplayEffect>(), 1.f,
                                ActorInfo->AbilitySystemComponent->MakeEffectContext());
                        }
                    }
                }

                Harvestable->Server_ApplyHarvest(FinalDamage, Character, SourceTagsToApply);
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
