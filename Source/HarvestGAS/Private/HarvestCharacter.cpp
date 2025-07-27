// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HarvestCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpec.h"
#include "HarvestGASCharacter.h"
#include "Public/PlayerHarvestAttributeSet.h"


AHarvestCharacter::AHarvestCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	Attributes = CreateDefaultSubobject<UPlayerHarvestAttributeSet>(TEXT("Attributes"));

	bReplicates = true;
}

UAbilitySystemComponent* AHarvestCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void AHarvestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GiveStartupAbilities();
	}
	InitAttributes();
}

void AHarvestCharacter::GiveStartupAbilities()
{
	if (AbilitySystem && HarvestAbilityClass)
	{
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(HarvestAbilityClass, 1, 0));
	}
}

void AHarvestCharacter::InitAttributes()
{
	if (Attributes)
	{
		Attributes->SetHarvestPower(1.0f);
		Attributes->SetYieldBonus(0.0f);
	}
}

void AHarvestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Looking
		// EnhancedInputComponent->BindAction(Harvest, ETriggerEvent::Triggered, this, &AHarvestCharacter::Input_Harvest);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AHarvestCharacter::Input_Harvest()
{
	if (AbilitySystem && HarvestAbilityClass)
	{
		AbilitySystem->TryActivateAbilityByClass(HarvestAbilityClass);
	}
}

