// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UPlayerOverlay;
class UPlayerHUDComponent;
class UWeaponData;
class AWeapon;
class UCombatComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponFirstReplicated, AWeapon*, Weapon);
UCLASS()
class RUNETERRA_API ABaseCharacter : public ACharacter, public IPlayerInterface, public IDamageableInterface
{
	GENERATED_BODY()
public:
	
	ABaseCharacter();
	
	UPROPERTY(BlueprintReadOnly)
	FTransform HANDLE_SocketTransform;

	bool bWeaponFirstReplicated;
	UPROPERTY(BlueprintAssignable)
	FWeaponFirstReplicated OnWeaponFirstReplicated;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aiming")
	float DefaultFieldOfView;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
	TArray<TObjectPtr<UAnimMontage>> DeathMontages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitReact")
	TArray<TObjectPtr<UAnimMontage>> HitReacts;
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void DeathEffects(AActor* DeathInstigator, UAnimMontage* DeathMontage);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HitReact(int32 MontageIndex);

protected:
	
#pragma region IPlayerInterface Functions

	virtual bool DoDamage_Implementation(float DamageAmount, AActor* DamageInstigator) override;
	virtual void EnableGameActions_Implementation(bool bEnable) override;
	virtual AWeapon* GetCurrentWeapon_Implementation() override;
	virtual FName GetWeaponAttachPoint_Implementation(const FGameplayTag& WeaponType) const override;
	virtual void Initiate_Crouch_Implementation() override;
	virtual void Initiate_Jump_Implementation() override;
	virtual bool IsDeadOrDying_Implementation() override;
	virtual void WeaponReplicated_Implementation() override;
	
#pragma endregion IPlayerInterface Functions
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
	
private:
	void Input_BasicAttack_Pressed();
	void Input_BasicAttack_Released();

	
	UFUNCTION()
	void OnDeathStarted(AActor* DyingActor, AActor* DeathInstigator);
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> Combat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> BasicAttackAction;

	bool bEnableGameActions;
};
