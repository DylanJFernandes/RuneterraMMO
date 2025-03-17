#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "RuneterraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class RUNETERRA_API URuneterraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URuneterraAttributeSet();

	// Offense Stats
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AbilityPower, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData AbilityPower;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, AbilityPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicPenetration, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData MagicPenetration;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, MagicPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackDamage, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, AttackDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Lethality, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Lethality;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, Lethality);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalStrikeChance, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData CriticalStrikeChance;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, CriticalStrikeChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalStrikeDamage, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData CriticalStrikeDamage;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, CriticalStrikeDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifeSteal, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData LifeSteal;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, LifeSteal);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpellVamp, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData SpellVamp;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, SpellVamp);

	// Defense Stats
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResist, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData MagicResist;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, MagicResist);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Tenacity, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Tenacity;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, Tenacity);

	// Utility Stats
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(URuneterraAttributeSet, MovementSpeed);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
	// Replication Functions
	UFUNCTION()
	void OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const;

	UFUNCTION()
	void OnRep_MagicPenetration(const FGameplayAttributeData& OldMagicPenetration) const;

	UFUNCTION()
	void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage) const;

	UFUNCTION()
	void OnRep_Lethality(const FGameplayAttributeData& OldLethality) const;

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;

	UFUNCTION()
	void OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance) const;

	UFUNCTION()
	void OnRep_CriticalStrikeDamage(const FGameplayAttributeData& OldCriticalStrikeDamage) const;

	UFUNCTION()
	void OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal) const;

	UFUNCTION()
	void OnRep_SpellVamp(const FGameplayAttributeData& OldSpellVamp) const;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_MagicResist(const FGameplayAttributeData& OldMagicResist) const;

	UFUNCTION()
	void OnRep_Tenacity(const FGameplayAttributeData& OldTenacity) const;

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const;
};
