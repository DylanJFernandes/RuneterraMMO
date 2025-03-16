// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Player/DS_PlayerState.h"
#include "RuneterraPlayerState.generated.h"

class URuneterraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RUNETERRA_API ARuneterraPlayerState : public ADS_PlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
 ARuneterraPlayerState();
public:

	virtual void BeginPlay() override;
	virtual void OnLogout(const FString& Username) override;
	virtual void OnRetrievePlayerStats(const FDSPlayerStatsResponse& RetrievePlayerStatsResponse);

	void SavePlayerStats(const FString& Username);
protected:

#pragma region IAbilitySystemInterface Functions
	virtual	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
#pragma endregion  IAbilitySystemInterface Functions

	UPROPERTY()
	URuneterraAbilitySystemComponent* AbilitySystemComponent;
private:

	UPROPERTY()
	FPlayerStats LocalPlayerStats{};
	
};
