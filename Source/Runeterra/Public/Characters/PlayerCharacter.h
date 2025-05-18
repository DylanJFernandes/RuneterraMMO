// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UPlayerOverlay;
class UPlayerHUDComponent;
class UWeaponData;
class AWeapon;
class UCombatComponent;
class UInputAction;

UCLASS()
class RUNETERRA_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;

private:

	void InitAbilitySystemComponent();
	
};
