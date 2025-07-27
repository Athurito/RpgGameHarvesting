// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "HarvestCharacter.generated.h"


class UInputAction;
class UAbilitySystemComponent;
class UPlayerHarvestAttributeSet;
class UGameplayAbility;

UCLASS()
class HARVESTGAS_API AHarvestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AHarvestCharacter();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY()
	UPlayerHarvestAttributeSet* Attributes;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> HarvestAbilityClass;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Input_Harvest();

protected:
	virtual void BeginPlay() override;

	void GiveStartupAbilities();
	void InitAttributes();

	

};
