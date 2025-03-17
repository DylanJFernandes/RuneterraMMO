// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseEnemyCharacter.h"
#include "AbilitySystem/RuneterraAbilitySystemComponent.h"



ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<URuneterraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
}

void ABaseEnemyCharacter::BeginPlay()
{
	
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	GiveDefaultAbilites();
	
}

UAbilitySystemComponent* ABaseEnemyCharacter::GetAbilitySystemComponent() const
{
	return Super::GetAbilitySystemComponent();
}


