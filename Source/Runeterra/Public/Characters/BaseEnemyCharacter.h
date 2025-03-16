// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseEnemyCharacter.generated.h"


UCLASS()
class RUNETERRA_API ABaseEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemyCharacter();

protected:
	
	virtual void BeginPlay() override;

#pragma region IAbilitySystemInterface Functions
	virtual	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
#pragma endregion  IAbilitySystemInterface Functions



};
