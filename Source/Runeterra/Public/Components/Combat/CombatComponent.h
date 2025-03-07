// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatDataTypes.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReticleChangedDelegate, UMaterialInstanceDynamic*, ReticleDynMatInst, const FReticleParams&, ReticleParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundFiredDelegate);

class UShooterOverlay;
class UWeaponData;
class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNETERRA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	
	void Initiate_BasicAttack_Pressed();
	void Initiate_BasicAttack_Released();

	
	UFUNCTION(BlueprintPure, Category = "Shooter|Health")
	static UCombatComponent* FindCombatComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCombatComponent>() : nullptr); }
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Reliable, Server)
	void ServerEquipWeapon(AWeapon* NewWeapon);

	UPROPERTY(BlueprintAssignable, Category=Combat)
	FReticleChangedDelegate OnReticleChanged;
	
	UPROPERTY(BlueprintAssignable, Category=Combat)
	FRoundFiredDelegate OnRoundFired;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWeaponData> WeaponData;
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon, BlueprintReadOnly)
	AWeapon* CurrentWeapon;
	
	UPROPERTY(EditDefaultsOnly)
	float TraceLength;
	
	UPROPERTY(BlueprintReadOnly)
	FPlayerHitResult Local_PlayerHitResult;

protected:
	virtual void BeginPlay() override;

private:
	void SetCurrentWeapon(AWeapon* NewWeapon, AWeapon* LastWeapon = nullptr);
	void AddWeapon(AWeapon* Weapon);
	void EquipWeapon(AWeapon* Weapon);
	void BasicAttackTimerFinished();	
	FVector HitScanTrace(float SweepRadius, FHitResult& OutHit);

	void Local_BasicAttack();

	UFUNCTION(Server, Reliable)
	void Server_BasicAttack(const FVector_NetQuantize& TraceStart, const FHitResult& Impact, bool bScoredHit, bool bHeadShot);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BasicAttack(const FHitResult& Impact);

	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* LastWeapon);
	
	bool bTriggerPressed;
	FTimerHandle FireTimer;

};