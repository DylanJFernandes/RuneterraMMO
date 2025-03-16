// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "BaseCharacter.generated.h"


class UGameplayAbility;
class UHealthComponent;
class URuneterraAbilitySystemComponent;

UCLASS()
class RUNETERRA_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface, public IDamageableInterface
{
	GENERATED_BODY()
public:

	ABaseCharacter();
	//~IAbilitySystemInterface interface

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
	TArray<TObjectPtr<UAnimMontage>> DeathMontages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitReact")
	TArray<TObjectPtr<UAnimMontage>> HitReacts;
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void DeathEffects(AActor* DeathInstigator, UAnimMontage* DeathMontage);
protected:
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void GiveDefaultAbilites();
	UPROPERTY()
	URuneterraAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilites;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;
	

private:
	UFUNCTION()
	void OnDeathStarted(AActor* DyingActor, AActor* DeathInstigator);
	

};
