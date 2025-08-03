// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GA_Harvest.h"

#include "HarvestableConfig.h"
#include "HarvestConfigRegistry.h"
#include "SkillAttributeSet.h"
#include "GameFramework/Character.h"
#include "Public/HarvestableComponent.h"


void UGA_Harvest::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
    // if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    // {
    //     EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    //     return;
    // }
    //
    // ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    // if (!Character) return;
    //
    // FVector Start = Character->GetActorLocation() + Character->GetActorForwardVector() * 50.f;
    // FVector End = Start + Character->GetActorForwardVector() * TraceDistance;
    //
    // FHitResult Hit;
    // if (Character->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    // {
    //     if (AActor* HitActor = Hit.GetActor())
    //     {
    //         if (UHarvestableComponent* Harvestable = HitActor->FindComponentByClass<UHarvestableComponent>())
    //         {
    //             float FinalDamage = BaseHarvestDamage;
    //
    //             if (const USkillAttributeSet* Skills = ActorInfo->AbilitySystemComponent->GetSet<USkillAttributeSet>())
    //             {
    //                 if (Harvestable->Config->ResourceType.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Resource.Stone"))))
    //                 {
    //                     FinalDamage *= (1.f + Skills->GetMiningLevel() * 0.05f);
    //                     
    //                     if (MiningXP_GE)
    //                     {
    //                         ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(+
    //                             MiningXP_GE->GetDefaultObject<UGameplayEffect>(), 1.f,
    //                             ActorInfo->AbilitySystemComponent->MakeEffectContext());
    //                     }
    //                 }
    //                 else if (Harvestable->Config->ResourceType.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Resource.Wood"))))
    //                 {
    //                     FinalDamage *= (1.f + Skills->GetWoodcuttingLevel() * 0.05f);
    //                     if (WoodcuttingXP_GE)
    //                     {
    //                         ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(
    //                             WoodcuttingXP_GE->GetDefaultObject<UGameplayEffect>(), 1.f,
    //                             ActorInfo->AbilitySystemComponent->MakeEffectContext());
    //                     }
    //                 }
    //             }
    //
    //             Harvestable->Server_ApplyHarvest(FinalDamage, Character, SourceTagsToApply);
    //         }
    //     }
    // }
    //
    // EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character) { EndAbility(Handle, ActorInfo, ActivationInfo, true, false); return; }

    // Perform line trace to find harvestable
    FVector Start = Character->GetActorLocation() + Character->GetActorForwardVector() * 50.f;
    FVector End = Start + Character->GetActorForwardVector() * TraceDistance;

    FHitResult Hit;
    UWorld* World = Character->GetWorld();
    if (!World || !World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    if (AActor* HitActor = Hit.GetActor())
    {
        if (UHarvestableComponent* Harvestable = HitActor->FindComponentByClass<UHarvestableComponent>())
        {
            
            // Calculate final damage based on skill
            float FinalDamage = Harvestable->Config->BaseDamage;
            if (const USkillAttributeSet* Skills = ActorInfo->AbilitySystemComponent->GetSet<USkillAttributeSet>())
            {
                // Dynamically select attribute via tag mapping in config
                const FGameplayTag& SkillTag = Harvestable->Config->SkillAttributeTag;
                float SkillLevel = 0.f;
                if (SkillTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Skill.Mining"))))
                {
                    SkillLevel = Skills->GetMiningLevel();
                }
                else if (SkillTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Skill.Woodcutting"))))
                {
                    SkillLevel = Skills->GetWoodcuttingLevel();
                }
                // Additional skill types can be handled similarly

                const float SkillMultiplier = Harvestable->Config->GetSkillMultiplierFromCurve(SkillLevel);
                FinalDamage *= (1.f + SkillLevel * SkillMultiplier);
                
                // Apply XP GameplayEffect if set
                if (Harvestable->Config->ExperienceGameplayEffect)
                {
                    ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(
                        Harvestable->Config->ExperienceGameplayEffect->GetDefaultObject<UGameplayEffect>(), 1.f,
                        ActorInfo->AbilitySystemComponent->MakeEffectContext());
                }
            }

            // Execute harvest
            Harvestable->Server_ApplyHarvest(FinalDamage, Character);
        }
    }

    // Finish ability
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
