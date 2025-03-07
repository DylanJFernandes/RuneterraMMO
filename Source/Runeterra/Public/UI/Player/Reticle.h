// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Combat/CombatDataTypes.h"
#include "Reticle.generated.h"

namespace Reticle
{
	extern RUNETERRA_API const FName Inner_RGBA;
	extern RUNETERRA_API const FName RoundedCornerScale;
	extern RUNETERRA_API const FName ShapeCutThickness;
}

class UImage;
class AWeapon;

UCLASS()
class RUNETERRA_API UReticle : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Reticle;

private:

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CurrentReticle_DynMatInst;

	FReticleParams CurrentReticleParams;
	float BaseCornerScaleFactor;
	float _BaseCornerScaleFactor_RoundFired;	
	float BaseShapeCutFactor;
	float _BaseShapeCutFactor_RoundFired;

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void OnReticleChange(UMaterialInstanceDynamic* ReticleDynMatInst, const FReticleParams& ReticleParams);
	
	UFUNCTION()
	void OnAttacked();

	UFUNCTION()
	void OnWeaponFirstReplicated(AWeapon* Weapon);
};
