// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Components/Combat/CombatDataTypes.h"
#include "Weapon.generated.h"

struct FPlayerHitResult;

/** Enum used to describe what state the weapon is currently in. */
UENUM(BlueprintType)
namespace EWeaponState
{
	enum Type
	{
		Idle,
		Attacking,
		Equipping,
	};
}

UENUM(BlueprintType)
namespace EAttackType
{
	enum Type
	{
		Auto,
		SemiAuto,
		Single
	};
}

UCLASS()
class RUNETERRA_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Instigator() override;

	/** perform initial setup */
	virtual void PostInitializeComponents() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag WeaponType;

	//////////////////////////////////////////////////////////////////////////
	// Firing the Weapon

	void Local_BasicAttack(const FVector& ImpactPoint, const FVector& ImpactNormal, TEnumAsByte<EPhysicalSurface> SurfaceType);
	void Rep_BasicAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void FireEffects(const FVector& ImpactPoint, const FVector& ImpactNormal, EPhysicalSurface SurfaceType);

	//////////////////////////////////////////////////////////////////////////
	// Inventory
	
	/** [server] weapon was added to pawn's inventory */
	virtual void OnEnterInventory(APawn* NewOwner);

	/** weapon is holstered by owner pawn */
	virtual void OnUnEquip();

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip(const AWeapon* LastWeapon);

	/** set the weapon's owning pawn */
	void SetOwningPawn(APawn* NewOwningPawn);

	/** get weapon mesh (needs pawn owner to determine variant) */
	USkeletalMeshComponent* GetWeaponMesh() const;
	
	/** update weapon state */
	void SetWeaponState(EWeaponState::Type NewState);

	EWeaponState::Type GetWeaponState() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Sequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HeadShotDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EAttackType::Type> AttackType;
	
	USkeletalMeshComponent* GetMesh() const;
	
	UMaterialInstanceDynamic* GetReticleDynamicMaterialInstance();
	UMaterialInstanceDynamic* GetWeaponIconDynamicMaterialInstance();

	UPROPERTY(EditDefaultsOnly, Category=Reticle)
	FReticleParams ReticleParams;

protected:
	virtual void BeginPlay() override;
	
	//////////////////////////////////////////////////////////////////////////
	// Inventory

	/** attaches weapon mesh to pawn's mesh */
	void AttachMeshToPawn();

	/** detaches weapon mesh from pawn */
	void DetachMeshFromPawn();

private:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** current weapon state */
	EWeaponState::Type CurrentState;

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	TObjectPtr<UMaterialInterface> ReticleMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMatInst_Reticle;

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	TObjectPtr<UMaterialInterface> WeaponIconMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMatInst_WeaponIcon;
	
};