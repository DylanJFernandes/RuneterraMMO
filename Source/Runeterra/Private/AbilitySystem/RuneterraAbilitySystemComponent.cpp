// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RuneterraAbilitySystemComponent.h"


// Sets default values for this component's properties
URuneterraAbilitySystemComponent::URuneterraAbilitySystemComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}


void URuneterraAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


