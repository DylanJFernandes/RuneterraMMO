// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Player/DS_PlayerState.h"
#include "RuneterraPlayerState.generated.h"

class URuneterraAbilitySystemComponent;
class URuneterraAttributeSet;
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
	virtual URuneterraAttributeSet* GetAttributeSet() const;
#pragma endregion  IAbilitySystemInterface Functions

	UPROPERTY()
	URuneterraAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<URuneterraAttributeSet> AttributeSet;
private:

	UPROPERTY()
	FPlayerStats LocalPlayerStats{};
	
};
