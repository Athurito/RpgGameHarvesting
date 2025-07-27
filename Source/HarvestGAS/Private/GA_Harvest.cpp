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

	FVector Start = Character->GetActorLocation() + Character->GetControlRotation().Vector() * 50.f;
	FVector End   = Start + Character->GetControlRotation().Vector() * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(GA_HarvestTrace), false, Character);

	bool bHit = Character->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	DrawDebugLine(Character->GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 2.f, 0, 1.f);
#endif

	if (bHit && Hit.GetActor())
	{
		if (UHarvestableComponent* Harvestable = Hit.GetActor()->FindComponentByClass<UHarvestableComponent>())
		{
			float Power = 1.f;
			if (const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
			{
				if (const UPlayerHarvestAttributeSet* Attr = ASC->GetSet<UPlayerHarvestAttributeSet>())
				{
					Power = Attr->GetHarvestPower();
				}
			}

			const float Amount = BaseHarvestDamage * Power;
			Harvestable->Server_ApplyHarvest(Amount, Character);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
