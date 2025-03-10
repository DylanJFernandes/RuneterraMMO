// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DS_PlayerState.h"
#include "RuneterraPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RUNETERRA_API ARuneterraPlayerState : public ADS_PlayerState
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void OnLogout(const FString& Username) override;
	virtual void OnRetrievePlayerStats(const FDSPlayerStatsResponse& RetrievePlayerStatsResponse);

	void SavePlayerStats(const FString& Username);

private:

	UPROPERTY()
	FPlayerStats LocalPlayerStats{};
};
