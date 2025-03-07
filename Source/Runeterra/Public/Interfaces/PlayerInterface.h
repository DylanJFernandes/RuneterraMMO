#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

struct FGameplayTag;
class AWeapon;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class RUNETERRA_API IPlayerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FName GetWeaponAttachPoint(const FGameplayTag& WeaponType) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetPawnMesh() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void Initiate_BasicAttack_Pressed();

	UFUNCTION(BlueprintNativeEvent)
	void Initiate_BasicAttack_Released();

	UFUNCTION(BlueprintNativeEvent)
	void Initiate_Crouch();

	UFUNCTION(BlueprintNativeEvent)
	void Initiate_Jump();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void WeaponReplicated();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableGameActions(bool bEnable);
};