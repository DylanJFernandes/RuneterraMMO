// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/RuneterraAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

URuneterraAttributeSet::URuneterraAttributeSet()
{
}

void URuneterraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, MagicResist, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, Tenacity, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, AbilityPower, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, MagicPenetration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, Lethality, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, CriticalStrikeChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, CriticalStrikeDamage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, LifeSteal, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URuneterraAttributeSet, SpellVamp, COND_None, REPNOTIFY_Always);
}

void URuneterraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }
}

void URuneterraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
}

// Replication Notification Functions
void URuneterraAttributeSet::OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, AbilityPower, OldAbilityPower);
}

void URuneterraAttributeSet::OnRep_MagicPenetration(const FGameplayAttributeData& OldMagicPenetration) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, MagicPenetration, OldMagicPenetration);
}

void URuneterraAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, AttackDamage, OldAttackDamage);
}

void URuneterraAttributeSet::OnRep_Lethality(const FGameplayAttributeData& OldLethality) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, Lethality, OldLethality);
}

void URuneterraAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, AttackSpeed, OldAttackSpeed);
}

void URuneterraAttributeSet::OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, CriticalStrikeChance, OldCriticalStrikeChance);
}

void URuneterraAttributeSet::OnRep_CriticalStrikeDamage(const FGameplayAttributeData& OldCriticalStrikeDamage) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, CriticalStrikeDamage, OldCriticalStrikeDamage);
}

void URuneterraAttributeSet::OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, LifeSteal, OldLifeSteal);
}

void URuneterraAttributeSet::OnRep_SpellVamp(const FGameplayAttributeData& OldSpellVamp) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, SpellVamp, OldSpellVamp);
}

void URuneterraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, Health, OldHealth);
}

void URuneterraAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, HealthRegen, OldHealthRegen);
}

void URuneterraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, MaxHealth, OldMaxHealth);
}


void URuneterraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, Armor, OldArmor);
}

void URuneterraAttributeSet::OnRep_MagicResist(const FGameplayAttributeData& OldMagicResist) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, MagicResist, OldMagicResist);
}

void URuneterraAttributeSet::OnRep_Tenacity(const FGameplayAttributeData& OldTenacity) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, Tenacity, OldTenacity);
}

void URuneterraAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URuneterraAttributeSet, MovementSpeed, OldMovementSpeed);
}
