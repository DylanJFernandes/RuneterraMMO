// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/Reticle.h"

#include "Characters/BaseCharacter.h"
#include "Components/Image.h"
#include "Components/Combat/CombatComponent.h"
#include "Weapon/Weapon.h"
#include "Interfaces/PlayerInterface.h"

namespace Reticle
{
	const FName Inner_RGBA = FName("Inner_RGBA");
	const FName RoundedCornerScale = FName("RoundedCornerScale");
	const FName ShapeCutThickness = FName("ShapeCutThickness");
}

void UReticle::NativeConstruct()
{
	Super::NativeConstruct();

	Image_Reticle->SetRenderOpacity(0.f);

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UReticle::OnPossessedPawnChanged);
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwningPlayer()->GetPawn());
	if (!IsValid(BaseCharacter)) return;
	
	OnPossessedPawnChanged(nullptr, BaseCharacter);
	
	if (BaseCharacter->bWeaponFirstReplicated)
	{
		AWeapon* CurrentWeapon = IPlayerInterface::Execute_GetCurrentWeapon(BaseCharacter);
		if (IsValid(CurrentWeapon))
		{
			OnReticleChange(CurrentWeapon->GetReticleDynamicMaterialInstance(), CurrentWeapon->ReticleParams);
		}
	}
	else
	{
		BaseCharacter->OnWeaponFirstReplicated.AddDynamic(this, &UReticle::OnWeaponFirstReplicated);
	}
}

void UReticle::OnWeaponFirstReplicated(AWeapon* Weapon)
{
	OnReticleChange(Weapon->GetReticleDynamicMaterialInstance(), Weapon->ReticleParams);
}

void UReticle::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	UCombatComponent* OldPawnCombat = UCombatComponent::FindCombatComponent(OldPawn);
	if (IsValid(OldPawnCombat))
	{
		OldPawnCombat->OnReticleChanged.RemoveDynamic(this, &UReticle::OnReticleChange);
		OldPawnCombat->OnRoundFired.RemoveDynamic(this, &UReticle::OnAttacked);
	}
	UCombatComponent* NewPawnCombat = UCombatComponent::FindCombatComponent(NewPawn);
	if (IsValid(NewPawnCombat))
	{
		Image_Reticle->SetRenderOpacity(1.f);
		NewPawnCombat->OnReticleChanged.AddDynamic(this, &UReticle::OnReticleChange);
		NewPawnCombat->OnRoundFired.AddDynamic(this, &UReticle::OnAttacked);
	}
}

void UReticle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	_BaseCornerScaleFactor_RoundFired = FMath::FInterpTo(_BaseCornerScaleFactor_RoundFired, 0.f, InDeltaTime, CurrentReticleParams.RoundFiredInterpSpeed);
	_BaseShapeCutFactor_RoundFired = FMath::FInterpTo(_BaseShapeCutFactor_RoundFired, 0.f, InDeltaTime, CurrentReticleParams.RoundFiredInterpSpeed);

	BaseCornerScaleFactor = _BaseCornerScaleFactor_RoundFired;
	BaseShapeCutFactor = _BaseShapeCutFactor_RoundFired;
	if (IsValid(CurrentReticle_DynMatInst))
	{
		CurrentReticle_DynMatInst->SetScalarParameterValue(Reticle::RoundedCornerScale, BaseCornerScaleFactor);
		CurrentReticle_DynMatInst->SetScalarParameterValue(Reticle::ShapeCutThickness, BaseShapeCutFactor);
	}
}

void UReticle::OnReticleChange(UMaterialInstanceDynamic* ReticleDynMatInst, const FReticleParams& ReticleParams)
{
	CurrentReticleParams = ReticleParams;
	
	CurrentReticle_DynMatInst = ReticleDynMatInst;
	FSlateBrush Brush;
	Brush.SetResourceObject(ReticleDynMatInst);
	if (IsValid(Image_Reticle))
	{
		Image_Reticle->SetBrush(Brush);
	}
	
}
void UReticle::OnAttacked()
{
	_BaseCornerScaleFactor_RoundFired += CurrentReticleParams.ScaleFactor_RoundFired;
	_BaseShapeCutFactor_RoundFired += CurrentReticleParams.ShapeCutFactor_RoundFired;
}
