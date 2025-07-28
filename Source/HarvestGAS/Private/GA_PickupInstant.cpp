// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PickupInstant.h"

#include "AbilitySystemComponent.h"
#include "HarvestableComponent.h"
#include "HarvestableConfig.h"
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
	if (!Character) return;

	FVector Start = Character->GetActorLocation();
	FVector End = Start + Character->GetActorForwardVector() * TraceDistance;

	FHitResult Hit;
	if (Character->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
	{
		if (UHarvestableComponent* Harvestable = Hit.GetActor()->FindComponentByClass<UHarvestableComponent>())
		{
			if (Harvestable->Config && Harvestable->Config->HarvestType == EHarvestType::InstantPickup)
			{
				Harvestable->Server_ApplyHarvest(0.f, Character, SourceTagsToApply); // 0 Schaden → triggert sofort
				if (XP_GE)
				{
					ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(
						XP_GE->GetDefaultObject<UGameplayEffect>(), 1.f,
						ActorInfo->AbilitySystemComponent->MakeEffectContext());
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
