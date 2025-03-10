// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/PlayerStats/PlayerStatsManager.h"
#include "DS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerStatsManager> PlayerStatsManagerClass;

	// Override these with save functionality.
	virtual void OnLogout(const FString& Username){};
	// virtual void LoadPlayerStats() const{};
	
protected:
	UFUNCTION()
	virtual void OnRetrievePlayerStats(const FDSPlayerStatsResponse& RetrievePlayerStatsResponse);
	virtual void BeginPlay() override;
	void RecordPlayerStats(const FDSPlayerStatsInput& PlayerStatsInput) const;
	void LoadPlayerStats() const;

	

private:

	UPROPERTY()
	TObjectPtr<UPlayerStatsManager> PlayerStatsManager;
};
