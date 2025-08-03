// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PickupInstant.h"

#include "AbilitySystemComponent.h"
#include "HarvestableComponent.h"
#include "HarvestableConfig.h"
#include "HarvestConfigRegistry.h"
#include "SkillAttributeSet.h"
#include "GameFramework/Character.h"

void UGA_PickupInstant::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
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
                
                // Apply XP GameplayEffect if set
            	if ( Harvestable->Config->ExperienceGameplayEffect)
            	{
            		ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(
						 Harvestable->Config->ExperienceGameplayEffect->GetDefaultObject<UGameplayEffect>(), 1.f,
						ActorInfo->AbilitySystemComponent->MakeEffectContext());
            	}
            }

            // Execute harvest
            Harvestable->Server_ApplyHarvest(0, Character);
        }
    }
}
