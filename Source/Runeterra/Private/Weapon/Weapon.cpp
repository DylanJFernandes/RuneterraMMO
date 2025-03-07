// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "Interfaces/PlayerInterface.h"


AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh->bReceivesDecals = false;
	Mesh->CastShadow = true;
	Mesh->SetHiddenInGame(false);
	SetRootComponent(Mesh);
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

}

USkeletalMeshComponent* AWeapon::GetMesh() const
{
	return Mesh;
}

UMaterialInstanceDynamic* AWeapon::GetReticleDynamicMaterialInstance()
{
	if (!IsValid(DynMatInst_Reticle))
	{
		DynMatInst_Reticle = UMaterialInstanceDynamic::Create(ReticleMaterial, this);
	}
	return DynMatInst_Reticle;
}

UMaterialInstanceDynamic* AWeapon::GetWeaponIconDynamicMaterialInstance()
{
	if (!IsValid(DynMatInst_WeaponIcon))
	{
		DynMatInst_WeaponIcon = UMaterialInstanceDynamic::Create(WeaponIconMaterial, this);
	}
	return DynMatInst_WeaponIcon;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::AttachMeshToPawn()
{
	APawn* MyPawn = GetInstigator();
	if (!IsValid(MyPawn) || !MyPawn->Implements<UPlayerInterface>()) return;
	
	// Remove and hide both first and third person meshes
	DetachMeshFromPawn();

	// For locally controlled players we attach both weapons and let the bOnlyOwnerSee, bOwnerNoSee flags deal with visibility.
	const FName AttachPoint = IPlayerInterface::Execute_GetWeaponAttachPoint(MyPawn, WeaponType);
	
	if (MyPawn->IsLocallyControlled())
	{
		USkeletalMeshComponent* PawnMesh = IPlayerInterface::Execute_GetPawnMesh(MyPawn); 
		Mesh->SetHiddenInGame(false);
		Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
	}
	else
	{
		USkeletalMeshComponent* UseWeaponMesh = GetWeaponMesh();
		USkeletalMeshComponent* UsePawnMesh = IPlayerInterface::Execute_GetPawnMesh(MyPawn);
		UseWeaponMesh->AttachToComponent(UsePawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
		UseWeaponMesh->SetHiddenInGame(false);
	}
	
}

void AWeapon::OnEnterInventory(APawn* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void AWeapon::OnUnEquip()
{
	DetachMeshFromPawn();
}

void AWeapon::OnEquip(const AWeapon* LastWeapon)
{
	AttachMeshToPawn();
}

void AWeapon::SetOwningPawn(APawn* NewOwningPawn)
{
	if (GetOwner() != NewOwningPawn)
	{
		SetInstigator(NewOwningPawn);
		
		// net owner for RPC calls
		SetOwner(NewOwningPawn);
	}
}

USkeletalMeshComponent* AWeapon::GetWeaponMesh() const
{
	if (GetOwner() == nullptr) return nullptr;
	if (!GetOwner()->Implements<UPlayerInterface>()) return nullptr;

	return Mesh;
}

void AWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	CurrentState = NewState;
}

EWeaponState::Type AWeapon::GetWeaponState() const
{
	return CurrentState;
}

void AWeapon::DetachMeshFromPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetHiddenInGame(true);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnRep_Instigator()
{
	Super::OnRep_Instigator();
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetachMeshFromPawn();
}

void AWeapon::Local_BasicAttack(const FVector& ImpactPoint, const FVector& ImpactNormal, TEnumAsByte<EPhysicalSurface> SurfaceType)
{

	FireEffects(ImpactPoint, ImpactNormal, SurfaceType);
	
	if (GetInstigator()->IsLocallyControlled())
	{
		++Sequence;
	}
	
}

void AWeapon::Rep_BasicAttack()
{
	if (GetInstigator()->IsLocallyControlled())
	{
		--Sequence;
	}
}