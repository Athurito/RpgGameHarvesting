// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GA_Harvest.h"

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
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FVector Start = Character->GetActorLocation() + Character->GetActorForwardVector() * 50.f;
	FVector End = Start + Character->GetActorForwardVector() * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	if (Character->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (UHarvestableComponent* Harvestable = HitActor->FindComponentByClass<UHarvestableComponent>())
			{
				Harvestable->Server_ApplyHarvest(BaseHarvestDamage, Character, FGameplayTagContainer());
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}