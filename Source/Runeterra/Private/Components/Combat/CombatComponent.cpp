// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/CombatComponent.h"

#include "Characters/BaseCharacter.h"
#include "Data/WeaponData.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"
#include "TimerManager.h"
#include "Interfaces/DamageableInterface.h"
#include "Kismet/GameplayStatics.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTriggerPressed = false;

	// Melee length
	TraceLength = 10.f;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, CurrentWeapon);
}

void UCombatComponent::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
	check(GetOwner());
	check(GetOwner()->Implements<UPlayerInterface>());
	IPlayerInterface::Execute_WeaponReplicated(GetOwner());
}

void UCombatComponent::ServerEquipWeapon_Implementation(AWeapon* NewWeapon)
{
	EquipWeapon(NewWeapon);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCombatComponent::SetCurrentWeapon(AWeapon* NewWeapon, AWeapon* LastWeapon)
{
	AWeapon* LocalLastWeapon = nullptr;

	if (IsValid(LastWeapon))
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	// unequip previous
	if (IsValid(LocalLastWeapon))
	{
		LocalLastWeapon->OnUnEquip();
	}

	// equip new one
	if (IsValid(NewWeapon))
	{
		NewWeapon->SetOwningPawn(Cast<APawn>(GetOwner()));
		NewWeapon->OnEquip(LastWeapon);
	}
	
}

void UCombatComponent::AddWeapon(AWeapon* Weapon)
{
	if (IsValid(Weapon) && IsValid(GetOwner()) && GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		Weapon->OnEnterInventory(Cast<APawn>(GetOwner()));
		//Inventory.AddUnique(Weapon);
	}
}

void UCombatComponent::EquipWeapon(AWeapon* Weapon)
{
	if (!IsValid(Weapon) || !IsValid(GetOwner())) return;
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
	else
	{
		ServerEquipWeapon(Weapon);
	}
}

void UCombatComponent::Initiate_BasicAttack_Pressed()
{
	bTriggerPressed = true;
	if (!IsValid(GetOwner())) return;
	if (GetOwner()->Implements<UDamageableInterface>() && IDamageableInterface::Execute_IsDeadOrDying(GetOwner()))
	{
		bTriggerPressed = false;
		return;
	}

	if (!IsValid(CurrentWeapon)) return;
	if (CurrentWeapon->GetWeaponState() == EWeaponState::Idle)
	{
		Local_BasicAttack();
	}
}

void UCombatComponent::Initiate_BasicAttack_Released()
{
	bTriggerPressed = false;
}

void UCombatComponent::Local_BasicAttack()
{
	if (!IsValid(WeaponData) || !IsValid(CurrentWeapon)) return;

	UAnimMontage* BasicAttackMontage = WeaponData->MeshMontages.FindChecked(CurrentWeapon->WeaponType).FireMontage;
	USkeletalMeshComponent* PlayerMesh = IPlayerInterface::Execute_GetPawnMesh(GetOwner());
	if (IsValid(BasicAttackMontage) && IsValid(PlayerMesh))
	{
		PlayerMesh->GetAnimInstance()->Montage_Play(BasicAttackMontage);
	}

	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (IsValid(OwningPawn) && OwningPawn->IsLocallyControlled())
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &UCombatComponent::BasicAttackTimerFinished, CurrentWeapon->AttackTime);

		FHitResult Hit;
		FVector TraceStart = HitScanTrace(5.f, Hit);
		// Tell the weapon the impact point, normal, and physical material
		EPhysicalSurface SurfaceType = Hit.PhysMaterial.IsValid(false) ? Hit.PhysMaterial->SurfaceType.GetValue() : EPhysicalSurface::SurfaceType1;
		CurrentWeapon->Local_BasicAttack(Hit.ImpactPoint, Hit.ImpactNormal, SurfaceType);

		// Send the server the hit info.
		const bool bHitDamageableActor = IsValid(Hit.GetActor()) ? Hit.GetActor()->Implements<UDamageableInterface>() : false;
		const bool bHeadShot = Hit.BoneName == "head";

		if (GetNetMode() == NM_Standalone) return;
		Server_BasicAttack(TraceStart, Hit, bHitDamageableActor, bHeadShot);
	}
}

void UCombatComponent::Server_BasicAttack_Implementation(const FVector_NetQuantize& TraceStart, const FHitResult& Impact, bool bScoredHit, bool bHeadShot)
{
	// Do your server-side rewind validation here...
	if (!IsValid(CurrentWeapon) || !IsValid(GetOwner())) return;
	float Damage = bHeadShot ? CurrentWeapon->HeadShotDamage : CurrentWeapon->Damage;
	
	if (IsValid(Impact.GetActor()) && Impact.GetActor()->Implements<UDamageableInterface>())
	{
		IDamageableInterface::Execute_DoDamage(Impact.GetActor(), Damage, GetOwner());
	}
	
	Multicast_BasicAttack(Impact);
}

void UCombatComponent::Multicast_BasicAttack_Implementation(const FHitResult& Impact)
{
	if (!IsValid(CurrentWeapon) || !IsValid(GetOwner())) return;
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		CurrentWeapon->Rep_BasicAttack();
	}
	else
	{
		EPhysicalSurface SurfaceType = Impact.PhysMaterial.IsValid(false) ? Impact.PhysMaterial->SurfaceType.GetValue() : SurfaceType1;
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->Local_BasicAttack(Impact.ImpactPoint, Impact.ImpactNormal, SurfaceType);

			if (IsValid(WeaponData))
			{
				UAnimMontage* BasicAttackMontage = WeaponData->MeshMontages.FindChecked(CurrentWeapon->WeaponType).FireMontage;
				USkeletalMeshComponent* PlayerMeshComponent = IPlayerInterface::Execute_GetPawnMesh(GetOwner());
				if (IsValid(PlayerMeshComponent) && IsValid(BasicAttackMontage))
				{
					PlayerMeshComponent->GetAnimInstance()->Montage_Play(BasicAttackMontage);
				}

			}
		}
	}
}

void UCombatComponent::BasicAttackTimerFinished()
{
	if (!IsValid(GetOwner())) return;
	if (GetOwner()->Implements<UDamageableInterface>() && IDamageableInterface::Execute_IsDeadOrDying(GetOwner()))
	{
		bTriggerPressed = false;
		return;
	}
	if (CurrentWeapon->AttackType == EAttackType::Type::Auto && bTriggerPressed)
	{
		Local_BasicAttack();
		return;
	}
	CurrentWeapon->SetWeaponState(EWeaponState::Idle);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwningPawn) || !OwningPawn->IsLocallyControlled()) return;

	if (APlayerController* PC = Cast<APlayerController>(OwningPawn->GetController()); IsValid(PC))
	{
		FVector2D ViewportSize = FVector2D::ZeroVector;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}
		FVector2D ReticleLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		FVector ReticleWorldLocation;
		FVector ReticleWorlDirection;
		UGameplayStatics::DeprojectScreenToWorld(PC, ReticleLocation, ReticleWorldLocation, ReticleWorlDirection);
		
		Local_PlayerHitResult.Start = ReticleWorldLocation;
		Local_PlayerHitResult.End = Local_PlayerHitResult.Start + ReticleWorlDirection * TraceLength;
		Local_PlayerHitResult.bHitPlayerLastFrame = Local_PlayerHitResult.bHitPlayer;

		FHitResult TraceHit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.AddIgnoredActor(CurrentWeapon);
		
		FCollisionResponseParams ResponseParams;
		ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
		ResponseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);
		ResponseParams.CollisionResponse.SetResponse(ECC_WorldDynamic, ECR_Block);
		ResponseParams.CollisionResponse.SetResponse(ECC_PhysicsBody, ECR_Block);
		ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
		
		// TODO: Change "ECC_GameTraceChannel1" to "ECC_Weapon"
		GetWorld()->LineTraceSingleByChannel(TraceHit, Local_PlayerHitResult.Start, Local_PlayerHitResult.End, ECC_GameTraceChannel1, QueryParams, ResponseParams);
			
		Local_PlayerHitResult.End = TraceHit.bBlockingHit ? TraceHit.ImpactPoint : Local_PlayerHitResult.End;
		if (IsValid(TraceHit.GetActor()) && TraceHit.GetActor()->Implements<UPlayerInterface>())
		{
			Local_PlayerHitResult.bHitPlayer = true;
		}
		else
		{
			Local_PlayerHitResult.bHitPlayer = false;
		}
		
		Local_PlayerHitResult.bHeadShot = TraceHit.bBlockingHit && TraceHit.BoneName == "head";
		
	}
}

FVector UCombatComponent::HitScanTrace(float SweepRadius, FHitResult& OutHit)
{
	FVector Start = GetOwner()->GetActorLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(CurrentWeapon);
	
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldDynamic, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_PhysicsBody, ECR_Block);

	FVector2D ViewportSize = FVector2D::ZeroVector;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	if (APlayerController* PC = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController()); IsValid(PC))
	{
		FVector2D ReticleLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		FVector ReticleWorldLocation;
		FVector ReticleWorlDirection;
		UGameplayStatics::DeprojectScreenToWorld(PC, ReticleLocation, ReticleWorldLocation, ReticleWorlDirection);

		Start = ReticleWorldLocation;
		FVector End = Start + ReticleWorlDirection * TraceLength;

		// TODO: Change "ECC_GameTraceChannel1" to "ECC_Weapon"
		const bool bHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(SweepRadius), QueryParams, ResponseParams);
		if (!bHit)
		{
			OutHit.ImpactPoint = End;
			OutHit.ImpactNormal = (Start - End).GetSafeNormal();
		}
	}
	return Start;
}